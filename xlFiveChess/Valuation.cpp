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

int Valuation::EvalLine(const LineInfo &li)
{
    return li.Count * 100 + li.Blocked.HeadRemain * 10 + li.Blocked.TailRemain * 10 - (li.Blocked.HolePos > 0 ? 1 : 0) * 1;
}

bool Valuation::LineComparor(const LineInfo &lhs, const LineInfo &rhs)
{
    return Valuation::EvalLine(lhs) > Valuation::EvalLine(rhs);
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

            for (int k = 0; k < Direction_CountOrNone; ++k)
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
                        ++li.Blocked.HeadRemain;
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
                            if (li.Blocked.HolePos == 0 && li.Blocked.TailRemain == 0)  // ����Ǵ����ϵ�һ�������գ���ʱ��Ϊ�п�λ��
                            {
                                li.Blocked.HolePos = l;
                            }
                            else if (li.Blocked.HolePos > 0 && li.Blocked.TailRemain == 0)    // �ڶ�������
                            {
                                if (l == li.Blocked.HolePos + 1)  // ��һ��Ҳ�ǿհף���ô��Ϊ���ӽ������������ն�����β��
                                {
                                    li.Blocked.TailRemain = 2;
                                    li.Blocked.HolePos = 0;
                                }
                                else // ��һ�����ǿհף�˵��֮ǰ�м�չ����������ˣ�����ֱ�ӿ�ʼ�����
                                {
                                    ++li.Blocked.TailRemain;
                                }
                            }
                            else if (li.Blocked.TailRemain > 0) // �Ѿ��ڼ���ĩβ�հ׼����ˣ�˵�������ˣ���������հ���
                            {
                                ++li.Blocked.TailRemain;
                            }
                        }
                        else
                        {
                            ++li.Blocked.TailRemain;
                        }
                    }
                    else if (color == data[i][j])
                    {
                        if (li.Blocked.TailRemain == 0)
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

                if (li.Count < nCount || li.Count + li.Blocked.HeadRemain + li.Blocked.TailRemain + (li.Blocked.HolePos > 0 ? 1 : 0) < WIN_LENGTH)
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

    return colorFound;
}
