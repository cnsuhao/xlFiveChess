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
    // 棋型定义
    enum Shader
    {
        Shader_Five,                // 五子
        Shader_DoubleAliveFour,     // 活四
        Shader_SingleAliveFour,     // 冲四
        Shader_DoubleAliveThree,    // 活三
        Shader_SingleAliveThree,    // 眠三
        Shader_DoubleAliveTwo,      // 活二
        Shader_SingleAliveTwo,      // 眠二
        Shader_DoubleAliveOne,      // 活一
        Shader_SingleAliveOne,      // 眠一
        Shader_More,                // 如果定义的 CHESS_LENGTH 不是 5，下面不再列出棋型名称，只要把评分表补全即可
    };

    enum State
    {
        State_Qblique = 0x01,
        State_HasHole = 0x02,
    };

    static const double VALUE_DEF[][4] =
    {
                                      //   直线无跳  斜线无跳  直线有跳  斜线有跳
        /* Shader_Five,                */ { 16384,   16384,   16384,   16384 },
        /* Shader_DoubleAliveFour,     */ { 8192,    8192,    1024,    1024  },
        /* Shader_SingleAliveFour,     */ { 1024,    1024,    768,     768   },
        /* Shader_DoubleAliveThree,    */ { 256,     256,     192,     192   },
        /* Shader_SingleAliveThree,    */ { 64,      64,      48,      48    },
        /* Shader_DoubleAliveTwo,      */ { 32,      32,      24,      24    },
        /* Shader_SingleAliveTwo,      */ { 8,       8,       6,       6     },
        /* Shader_DoubleAliveOne,      */ { 4,       4,       4,       4     },
        /* Shader_SingleAliveOne,      */ { 1,       1,       -1,      -1    },
    };

    Shader shader = Shader_More;
    unsigned int dwState = 0;

    if (li.Count >= CHESS_LENGTH && (li.Blank.HolePos == 0 || li.Blank.HolePos >= CHESS_LENGTH))
    {
        shader = Shader_Five;
    }
    else
    {
        for (int i = 1; i < _countof(VALUE_DEF) - 1; i += 2)
        {
            int n = (i + 1) / 2;

            if (li.Count >= CHESS_LENGTH - n && li.Blank.HeadRemain + li.Blank.TailRemain + (li.Blank.HolePos > 0 ? 1 : 0) > n && li.Blank.HeadRemain > 0 && li.Blank.TailRemain > 0)
            {
                shader = (Shader)i;
                break;
            }
            if (li.Count >= CHESS_LENGTH - n && li.Blank.HeadRemain + li.Blank.TailRemain + (li.Blank.HolePos > 0 ? 1 : 0) >= n)
            {
                shader = (Shader)(i + 1);
                break;
            }
        }
    }

    if (li.Direction == Direction_DownLeft || li.Direction == Direction_DownRight)
    {
        dwState |= State_Qblique;
    }
    if (li.Blank.HolePos > 0)
    {
        dwState |= State_HasHole;
    }

    double dValue = VALUE_DEF[shader][dwState];
    XL_ASSERT(dValue > 0);
    return dValue;
}

double Valuation::EvalChessboard(const ChessData &data, ChessmanColor colorToEval, double *pOppsiteValue)
{
    LineInfoCollection lic;
    Valuation::FindLine(data, 1, ChessmanColor_None, true, true, &lic);

    double dValues[] =
    {
        /* ChessmanColor_None  => */ 0.0,
        /* ChessmanColor_Black => */ 0.0,
        /* ChessmanColor_White => */ 0.0,
    };

    for (LineInfoCollection::Iterator it = lic.Begin(); it != lic.End(); ++it)
    {
        dValues[it->Color] += EvalLine(*it);
    }

    if (pOppsiteValue != nullptr)
    {
        *pOppsiteValue = dValues[!colorToEval];
    }

    return dValues[colorToEval];
}

ChessmanColor Valuation::FindLine(const ChessData &data, int nCount, ChessmanColor colorToFind, bool bFindAll, bool bAllowHole, LineInfoCollection *pResult)
{
    if (pResult != nullptr)
    {
        pResult->Clear();
    }

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
                    if (data[i - DirectionDef[k].x][j - DirectionDef[k].y] == data[i][j])   // 找过了
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

                // 往前面扫描，确定起始处空白数
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

                // 往后面扫描，确定连子数、中空位置以及末尾空白数
                for (int l = 1; l < CHESSBOARD_SIZE; ++l)
                {
                    if (i + DirectionDef[k].x * l >= CHESSBOARD_SIZE ||
                        i + DirectionDef[k].x * l < 0 ||
                        j + DirectionDef[k].y * l >= CHESSBOARD_SIZE)
                    {
                        if (li.Blank.HolePos > 0 && li.Blank.TailRemain == 0 && l == li.Blank.HolePos + 1)
                        {
                            li.Blank.TailRemain = 1;
                            li.Blank.HolePos = 0;
                        }

                        break;
                    }

                    ChessmanColor color = data[i + DirectionDef[k].x * l][j + DirectionDef[k].y * l];

                    if (color == ChessmanColor_None)
                    {
                        if (bAllowHole)
                        {
                            if (li.Blank.HolePos == 0 && li.Blank.TailRemain == 0)  // 如果是此线上第一次遇到空，暂时记为中空位置
                            {
                                li.Blank.HolePos = l;
                            }
                            else if (li.Blank.HolePos > 0 && li.Blank.TailRemain == 0)    // 第二次遇到
                            {
                                if (l == li.Blank.HolePos + 1)  // 上一个也是空白，那么认为连子结束，这两个空都属于尾空
                                {
                                    li.Blank.TailRemain = 2;
                                    li.Blank.HolePos = 0;
                                }
                                else // 上一个不是空白，说明之前中间空过后又连上了，现在直接开始算结束
                                {
                                    ++li.Blank.TailRemain;
                                }
                            }
                            else if (li.Blank.TailRemain > 0) // 已经在计算末尾空白计数了，说明结束了，继续计算空白数
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
                        if (l == li.Blank.HolePos + 1)  // 上一个是空白，那么认为连子结束，上个空属于尾空
                        {
                            li.Blank.TailRemain = 1;
                            li.Blank.HolePos = 0;
                        }

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
