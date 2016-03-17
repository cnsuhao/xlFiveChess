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

int Valuation::EvalLine(const LineInfo &li)
{
    return li.Count * 100 + li.Blocked.HeadBlocked * (-1) + li.Blocked.TailBlocked * (-1);
}

bool Valuation::LineComparor(const LineInfo &lhs, const LineInfo &rhs)
{
    return Valuation::EvalLine(lhs) > Valuation::EvalLine(rhs);
}

ChessmanColor Valuation::FindLine(const ChessData &data, int nCount, ChessmanColor colorToFind, bool bFindAll, LineInfoCollection *pResult)
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
                    if (data[i - DirectionDef[k].x][j - DirectionDef[k].y] == data[i][j])   // ÕÒ¹ýÁË
                    {
                        continue;
                    }
                }

                LineInfo li =
                {
                    1,
                    (Direction)k,
                    { false, false },
                    { i, j },
                };

                if (i - DirectionDef[k].x >= 0 &&
                    i - DirectionDef[k].x < CHESSBOARD_SIZE &&
                    j - DirectionDef[k].y >= 0)
                {
                    li.Blocked.HeadBlocked = (data[i - DirectionDef[k].x][j - DirectionDef[k].y] != ChessmanColor_None);
                }
                else
                {
                    li.Blocked.HeadBlocked = true;
                }

                for (int l = 1; l < CHESSBOARD_SIZE; ++l)
                {
                    if (i + DirectionDef[k].x * l >= CHESSBOARD_SIZE ||
                        i + DirectionDef[k].x * l < 0 ||
                        j + DirectionDef[k].y * l >= CHESSBOARD_SIZE)
                    {
                        li.Blocked.TailBlocked = true;
                        break;
                    }

                    ChessmanColor color = data[i + DirectionDef[k].x * l][j + DirectionDef[k].y * l];

                    if (color == data[i][j])
                    {
                        ++li.Count;
                    }
                    else
                    {
                        li.Blocked.TailBlocked = (color != ChessmanColor_None);
                        break;
                    }
                }

                if (li.Count >= nCount)
                {
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
    }

    return colorFound;
}
