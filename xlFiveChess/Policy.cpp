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
#include <time.h>
#include <float.h>
#include "Log.h"

#define LOG_LINE(li)                                                                \
    XL_LOG_INFO(L"%d-line, %s%s, %s, L%dR%dM%d, %d",                                \
                (li).Count,                                                         \
                COORD_TAG_HORZ[(li).Position.x], COORD_TAG_VERT[(li).Position.y],   \
                DIRECTION_TAG[(li).Direction],                                      \
                (li).Blank.HeadRemain, (li).Blank.TailRemain, (li).Blank.HolePos,   \
                Valuation::EvalLine((li)))



struct IPolicy
{
    virtual ~IPolicy()
    {

    }

    virtual Point FindNextMove(const ChessData &data, ChessmanColor currentTurn) = 0;
};

////////////////////////////////////////////////////////////////////////////////
// AI，预测一步

class PolicyForecastAI : public IPolicy
{
public:
    Point FindNextMove(const ChessData &data, ChessmanColor currentTurn) override
    {
        XL_LOG_INFO_FUNCTION();

        ChessData &d = (ChessData &)data;

        Point ptOurs = INVALID_POSITION;    // 我方最佳点
        double dOurScoreMax = -DBL_MAX;     // 我方下在最佳点后的我方局面评分
        Point ptTheirs = INVALID_POSITION;  // 对方最佳点
        double dTheirScoreMax = -DBL_MAX;   // 对方下在最佳点后的对方局面评分

        for (int i = 0; i < CHESSBOARD_SIZE; ++i)
        {
            for (int j = 0; j < CHESSBOARD_SIZE; ++j)
            {
                if (data[i][j] == ChessmanColor_None)
                {
                    d[i][j] = currentTurn;
                    double dOurScore = Valuation::EvalChessboard(d, currentTurn);
                    d[i][j] = !currentTurn;
                    double dTheirScore = -Valuation::EvalChessboard(d, currentTurn);
                    d[i][j] = ChessmanColor_None;
                    XL_LOG_INFO(L"Pos:%s%s, OurMove: %lf, TheirMove: %lf", COORD_TAG_HORZ[i], COORD_TAG_VERT[j], dOurScore, dTheirScore);

                    if (dOurScore > dOurScoreMax)
                    {
                        dOurScoreMax = dOurScore;
                        ptOurs = Point(i, j);
                    }

                    if (dTheirScore > dTheirScoreMax)
                    {
                        dTheirScoreMax = dTheirScore;
                        ptTheirs = Point(i, j);
                    }
                }
            }
        }

        XL_LOG_INFO(L"Pos:%s%s, OurScoreMax:%lf", COORD_TAG_HORZ[ptOurs.x], COORD_TAG_VERT[ptOurs.y], dOurScoreMax);
        XL_LOG_INFO(L"Pos:%s%s, TheirScoreMax:%lf", COORD_TAG_HORZ[ptTheirs.x], COORD_TAG_VERT[ptTheirs.y], dTheirScoreMax);
        return dOurScoreMax >= dTheirScoreMax ? ptOurs : ptTheirs;
    }
};

////////////////////////////////////////////////////////////////////////////////
// 简单 AI，尽量连自己或者堵对方

class PolicySimpleAI : public IPolicy
{
public:
    Point FindNextMove(const ChessData &data, ChessmanColor currentTurn) override
    {
        XL_LOG_INFO_FUNCTION();

        LineInfoCollection ours, theirs;
        Valuation::FindLine(data, 1, currentTurn, true, true, &ours);
        Valuation::FindLine(data, 1, !currentTurn, true, true, &theirs);

        LineInfoCollection::Iterator itOurs = ours.Begin();
        LineInfoCollection::Iterator itTheirs = theirs.Begin();

        for (int i = CHESS_LENGTH - 1; i > 0; --i)
        {
            // 如果自己有 i 子，那么连自己

            XL_LOG_INFO(L"Find our %d-line", i);
            Point pt = FindNextMove(itOurs, ours.End(), i);

            if (pt != INVALID_POSITION)
            {
                return pt;
            }

            // 如果对方有 i 子，那么堵对方

            XL_LOG_INFO(L"Find their %d-line", i);
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
                LOG_LINE(*it);
                return it->Position + DirectionDef[it->Direction] * it->Blank.HolePos;
            }
            if (it->Blank.HeadRemain > 0 && it->Blank.HeadRemain >= it->Blank.TailRemain)
            {
                LOG_LINE(*it);
                return it->Position - DirectionDef[it->Direction];
            }
            if (it->Blank.TailRemain > 0 && it->Blank.TailRemain > it->Blank.HeadRemain)
            {
                LOG_LINE(*it);
                return it->Position + DirectionDef[it->Direction] * (it->Count + (it->Blank.HolePos > 0 ? 1 : 0));
            }
            if (it->Blank.HeadRemain > 0 && it->Blank.TailRemain)
            {
                LOG_LINE(*it);
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

Point Policy::FindNextMove(PolicyName policy, const ChessData &data, ChessmanColor currentTurn)
{
    static IPolicy *policies[Policy_Count] =
    {
        /* Policy_DorecastAI          => */ new PolicyForecastAI,
        /* Policy_SimpleAI            => */ new PolicySimpleAI,
        /* Policy_Random              => */ new PolicyRandom,
        /* Policy_Center              => */ new PolicyCenter,
    };

    return policies[policy]->FindNextMove(data, currentTurn);
}
