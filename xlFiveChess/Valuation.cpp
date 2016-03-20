//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   Valuation.cpp
//    Author:      Streamlet
//    Create Time: 2016-03-17
//    Description: 
//
//------------------------------------------------------------------------------


#include "Valuation.h"
#include <xl/Common/Meta/xlAssert.h>
#include <xl/Common/Algorithm/xlSort.h>
#include <math.h>
#include "Log.h"

double Valuation::EvalLine(const LineInfo &li)
{
    double dScore = 0;

    dScore += pow(10000.0, li.Count);
    dScore *= 100 * ((li.Blank.HeadRemain > 0 ? 1 : 0) + (li.Blank.TailRemain > 0 ? 1 : 0) - (li.Blank.HolePos > 0 ? 1 : 0));
    dScore *= 10 * (li.Blank.HeadRemain + li.Blank.TailRemain);
    dScore *= 10 * (abs(DirectionDef[li.Direction].x) + abs(DirectionDef[li.Direction].y));

    return dScore;
}

double Valuation::EvalChessboard(const ChessData &data, ChessmanColor colorToEval)
{
    LineInfoCollection lic;
    Valuation::FindLine(data, 1, ChessmanColor_None, true, true, &lic);

    double nScore[] =
    {
        /* ChessmanColor_None  => */ 0.0,
        /* ChessmanColor_Black => */ 0.0,
        /* ChessmanColor_White => */ 0.0,
    };

    for (LineInfoCollection::Iterator it = lic.Begin(); it != lic.End(); ++it)
    {
        nScore[it->Color] += EvalLine(*it);
    }

    return nScore[colorToEval] - nScore[!colorToEval];
}

ChessmanColor Valuation::FindLine(const ChessData &data, int nCount, ChessmanColor colorToFind, bool bFindAll, bool bAllowHole, LineInfoCollection *pResult)
{
    ChessmanColor colorFound = ChessmanColor_None;

    for (int i = 0; i < CHESSBOARD_SIZE; ++i)
    {
        for (int j = 0; j < CHESSBOARD_SIZE; ++j)
        {
            if (data[i][j] == ChessmanColor_None || (colorToFind != ChessmanColor_None && data[i][j] != colorToFind))
            {
                continue;
            }

            for (int k = 0; k < Direction_Count; ++k)
            {
                if (i + DirectionDef[k].x * (nCount - 1) >= CHESSBOARD_SIZE ||
                    i + DirectionDef[k].x * (nCount - 1) < 0 ||
                    j + DirectionDef[k].y * (nCount - 1) >= CHESSBOARD_SIZE)
                {
                    continue;
                }

                if (i - DirectionDef[k].x >= 0 &&
                    i - DirectionDef[k].x < CHESSBOARD_SIZE &&
                    j - DirectionDef[k].y >= 0)
                {
                    if (data[i - DirectionDef[k].x][j - DirectionDef[k].y] == data[i][j])   // �ҹ���
                    {
                        continue;
                    }
                }

                LineInfo li =
                {
                    data[i][j],
                    1,
                    (Direction)k,
                    { 0, 0, 0 },
                    { i, j },
                };

                // ��ǰ��ɨ�裬ȷ����ʼ���հ���
                for (int l = 1; l < CHESSBOARD_SIZE; ++l)
                {
                    if (i - DirectionDef[k].x * l < 0 ||
                        i - DirectionDef[k].x * l >= CHESSBOARD_SIZE ||
                        j - DirectionDef[k].y * l < 0)
                    {
                        break;
                    }

                    ChessmanColor color = data[i - DirectionDef[k].x * l][j - DirectionDef[k].y * l];

                    if (color == ChessmanColor_None)
                    {
                        ++li.Blank.HeadRemain;
                    }
                    else
                    {
                        break;
                    }
                }

                // ������ɨ�裬ȷ�����������п�λ���Լ�ĩβ�հ���
                for (int l = 1; l < CHESSBOARD_SIZE; ++l)
                {
                    if (i + DirectionDef[k].x * l >= CHESSBOARD_SIZE ||
                        i + DirectionDef[k].x * l < 0 ||
                        j + DirectionDef[k].y * l >= CHESSBOARD_SIZE)
                    {
                        break;
                    }

                    ChessmanColor color = data[i + DirectionDef[k].x * l][j + DirectionDef[k].y * l];

                    if (color == ChessmanColor_None)
                    {
                        if (bAllowHole)
                        {
                            if (li.Blank.HolePos == 0 && li.Blank.TailRemain == 0)  // ����Ǵ����ϵ�һ�������գ���ʱ��Ϊ�п�λ��
                            {
                                li.Blank.HolePos = l;
                            }
                            else if (li.Blank.HolePos > 0 && li.Blank.TailRemain == 0)    // �ڶ�������
                            {
                                if (l == li.Blank.HolePos + 1)  // ��һ��Ҳ�ǿհף���ô��Ϊ���ӽ������������ն�����β��
                                {
                                    li.Blank.TailRemain = 2;
                                    li.Blank.HolePos = 0;
                                }
                                else // ��һ�����ǿհף�˵��֮ǰ�м�չ����������ˣ�����ֱ�ӿ�ʼ�����
                                {
                                    ++li.Blank.TailRemain;
                                }
                            }
                            else if (li.Blank.TailRemain > 0) // �Ѿ��ڼ���ĩβ�հ׼����ˣ�˵�������ˣ���������հ���
                            {
                                ++li.Blank.TailRemain;
                            }
                        }
                        else
                        {
                            ++li.Blank.TailRemain;
                        }
                    }
                    else if (color == data[i][j])
                    {
                        if (li.Blank.TailRemain == 0)
                        {
                            ++li.Count;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                if (li.Count < nCount || li.Count + li.Blank.HeadRemain + li.Blank.TailRemain + (li.Blank.HolePos > 0 ? 1 : 0) < CHESS_LENGTH)
                {
                    continue;
                }

                colorFound = data[i][j];

                if (pResult != nullptr)
                {
                    pResult->PushBack(li);
                }

                if (!bFindAll)
                {
                    return colorFound;
                }

            }
        }
    }

    if (pResult != nullptr && pResult->Size() > 1)
    {
        xl::Sort::QuickSort(&(*pResult)[0], pResult->Size(), xl::Function<bool(const LineInfo &, const LineInfo &)>([](const LineInfo &lhs, const LineInfo &rhs)
        {
            return Valuation::EvalLine(lhs) > Valuation::EvalLine(rhs);
        }));
    }

    return colorFound;
}
