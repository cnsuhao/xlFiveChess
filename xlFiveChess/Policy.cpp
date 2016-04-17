//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   Policy.cpp
//    Author:      Streamlet
//    Create Time: 2016-03-17
//    Description: 
//
//------------------------------------------------------------------------------


#include "Policy.h"
#include "Valuation.h"
#include <Windows.h>
#include <time.h>
#include <float.h>
#include <stdlib.h>
#include "Log.h"

struct IPolicy
{
    virtual ~IPolicy()
    {

    }

    virtual Point FindNextMove(const ChessData &data, ChessmanColor currentTurn) = 0;

};

#ifdef DRAW_DEBUG_INFO

static LineInfoCollection g_LastLineInfoCollection;
static ChessboardValue g_LastChessboardValue;

#endif

////////////////////////////////////////////////////////////////////////////////
// AI，预测一步

class PolicyMinMaxSearch : public IPolicy
{
public:
    Point FindNextMove(const ChessData &data, ChessmanColor currentTurn) override
    {

        ChessData &d = (ChessData &)data;
        Point pt = INVALID_POSITION;
        FindMinMax(d, currentTurn, DBL_MAX, 0, &pt);
        return pt;
    }

private:
    double FindMinMax(ChessData &data, ChessmanColor currentTurn, double dAplhaPruning, int nDeep, Point *pPoint = nullptr)
    {
#ifdef DRAW_DEBUG_INFO
        if (nDeep == 0)
        {
            Policy::ClearLastChessboardValues();
            Valuation::FindLine(data, 1, ChessmanColor_None, true, true, &g_LastLineInfoCollection);
        }
#endif

        Point pt = INVALID_POSITION;    // 最佳点
        double dValueMax = -DBL_MAX;    // 下在最佳点后的局面评分
        bool bBreak = false;

        for (int i = 0; i < CHESSBOARD_SIZE && !bBreak; ++i)
        {
            for (int j = 0; j < CHESSBOARD_SIZE && !bBreak; ++j)
            {
                if (data[i][j] != ChessmanColor_None)
                {
                    continue;
                }

                double dValueOfPoint = 0;
                if (nDeep >= MAX_DEEP)
                {
                    data[i][j] = currentTurn;
                    dValueOfPoint = Valuation::EvalChessboard(data, currentTurn);
                    data[i][j] = ChessmanColor_None;
                }
                else
                {
                    data[i][j] = currentTurn;
                    Point pt;
                    dValueOfPoint = -FindMinMax(data, !currentTurn, -dValueMax, nDeep + 1, &pt);
                    data[i][j] = ChessmanColor_None;

                    if (dValueOfPoint > dAplhaPruning)
                    {
                        bBreak = true;
                        break;
                    }

                }

#ifdef DRAW_DEBUG_INFO
                if (nDeep == 0)
                {
                    g_LastChessboardValue[i][j] = dValueOfPoint;
                }
#endif

                if (dValueOfPoint > dValueMax ||
                   dValueOfPoint == dValueMax && max(abs(i - CHESSBOARD_SIZE / 2), abs(j - CHESSBOARD_SIZE / 2)) < max(abs(pt.x - CHESSBOARD_SIZE / 2), abs(pt.y - CHESSBOARD_SIZE / 2)))
                {
                    dValueMax = dValueOfPoint;
                    pt = Point(i, j);
                }
            }
        }

        if (pPoint != nullptr)
        {
            *pPoint = pt;
        }

        return dValueMax;
    }

private:
    static const int MAX_DEEP = 2;
};

////////////////////////////////////////////////////////////////////////////////
// 简单 AI，尽量连自己或者堵对方

class PolicySimpleAI : public IPolicy
{
public:
    Point FindNextMove(const ChessData &data, ChessmanColor currentTurn) override
    {
        LineInfoCollection ours, theirs;
        Valuation::FindLine(data, 1, currentTurn, true, true, &ours);
        Valuation::FindLine(data, 1, !currentTurn, true, true, &theirs);

#ifdef DRAW_DEBUG_INFO
        Policy::ClearLastChessboardValues();
        g_LastLineInfoCollection.Insert(g_LastLineInfoCollection.End(), ours.Begin(), ours.End());
        g_LastLineInfoCollection.Insert(g_LastLineInfoCollection.End(), theirs.Begin(), theirs.End());
#endif

        LineInfoCollection::Iterator itOurs = ours.Begin();
        LineInfoCollection::Iterator itTheirs = theirs.Begin();

        for (int i = CHESS_LENGTH - 1; i > 0; --i)
        {
            // 如果自己有 i 子，那么连自己

            Point pt = FindNextMove(itOurs, ours.End(), i);

            if (pt != INVALID_POSITION)
            {
                return pt;
            }

            // 如果对方有 i 子，那么堵对方

            pt = FindNextMove(itTheirs, theirs.End(), i);

            if (pt != INVALID_POSITION)
            {
                return pt;
            }
        }

        return INVALID_POSITION;
    }

private:
    Point FindNextMove(LineInfoCollection::Iterator &it, LineInfoCollection::Iterator &itEnd, int nMinCount)
    {
        while (it != itEnd)
        {
            if (it->Count < nMinCount)
            {
                break;
            }
            if (it->Blank.HolePos > 0)
            {
                return it->Position + DirectionDef[it->Direction] * it->Blank.HolePos;
            }
            if (it->Blank.HeadRemain > 0 && it->Blank.HeadRemain >= it->Blank.TailRemain)
            {
                return it->Position - DirectionDef[it->Direction];
            }
            if (it->Blank.TailRemain > 0 && it->Blank.TailRemain > it->Blank.HeadRemain)
            {
                return it->Position + DirectionDef[it->Direction] * (it->Count + (it->Blank.HolePos > 0 ? 1 : 0));
            }
            if (it->Blank.HeadRemain > 0 && it->Blank.TailRemain)
            {
                return it->Position - DirectionDef[it->Direction];
            }

            ++it;
        }

        return INVALID_POSITION;
    }
};

////////////////////////////////////////////////////////////////////////////////
// 完全随机

class PolicyRandom : public IPolicy
{
public:
    PolicyRandom(int nMaxTryTime = 100) : m_nMaxTryTime(nMaxTryTime)
    {
        srand((unsigned int)time(nullptr));
    }

    Point FindNextMove(const ChessData &data, ChessmanColor currentTurn) override
    {
        Point pt;

        for (int i = 0; i < m_nMaxTryTime; ++i)
        {
            pt.x = rand() % CHESSBOARD_SIZE;
            pt.y = rand() % CHESSBOARD_SIZE;

            if (data[pt.x][pt.y] == ChessmanColor_None)
            {
                return pt;
            }
        }

        return INVALID_POSITION;
    }

private:
    int m_nMaxTryTime;
};

////////////////////////////////////////////////////////////////////////////////
// 尽量靠近中心点下

class PolicyCenter : public IPolicy
{
    Point FindNextMove(const ChessData &data, ChessmanColor currentTurn) override
    {
        Point pt(CHESSBOARD_SIZE / 2, CHESSBOARD_SIZE / 2);

        if (data[pt.x][pt.y] == ChessmanColor_None)
        {
            return pt;
        }

        for (int i = 1; i < CHESSBOARD_SIZE / 2; ++i)
        {
            Point delta[] =
            {
                {  i, -i },
                { -i, -i },
                { -i,  i },
                {  i,  i },
                { -i,  0 },
                {  0, -i },
                {  i,  0 },
                {  0,  i },
            };

            for (int j = 0; j < _countof(delta); ++j)
            {
                Point ptDelta = pt + delta[j];

                if (data[ptDelta.x][ptDelta.y] == ChessmanColor_None)
                {
                    return pt;
                }
            }
        }

        return INVALID_POSITION;
    }
};
////////////////////////////////////////////////////////////////////////////////

static IPolicy *g_Policies[Policy_Count] =
{
    /* Policy_MinMaxSearch        => */ new PolicyMinMaxSearch,
    /* Policy_SimpleAI            => */ new PolicySimpleAI,
    /* Policy_Random              => */ new PolicyRandom,
    /* Policy_Center              => */ new PolicyCenter,
};

Point Policy::FindNextMove(PolicyName policy, const ChessData &data, ChessmanColor currentTurn)
{
    return g_Policies[policy]->FindNextMove(data, currentTurn);
}

#ifdef DRAW_DEBUG_INFO

const LineInfoCollection &Policy::GetLastLineInfoCollection()
{
    return g_LastLineInfoCollection;
}

const ChessboardValue &Policy::GetLastChessboardValues()
{
    return g_LastChessboardValue;
}

void Policy::ClearLastLineInfoCollection()
{
    g_LastLineInfoCollection.Clear();
}

void Policy::ClearLastChessboardValues()
{
    memset(g_LastChessboardValue, ChessmanColor_None, sizeof(g_LastChessboardValue));
}

#endif
