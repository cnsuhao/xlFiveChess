//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   FiveModel.h
//    Author:      Streamlet
//    Create Time: 2016-03-17
//    Description: 
//
//------------------------------------------------------------------------------

#ifndef __FIVEMODEL_H_1FE257EC_AAA2_43D3_B918_BF9AE7FAE21B_INCLUDED__
#define __FIVEMODEL_H_1FE257EC_AAA2_43D3_B918_BF9AE7FAE21B_INCLUDED__


#include <xl/Common/Containers/xlArray.h>
#include <xl/Common/Math/xlMathBase.h>

static const int CHESSBOARD_SIZE    = 15;
static const int CHESS_LENGTH         = 5;

static const wchar_t * const COORD_TAG_HORZ[CHESSBOARD_SIZE] = { L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O" };
static const wchar_t * const COORD_TAG_VERT[CHESSBOARD_SIZE] = { L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"11", L"12", L"13", L"14", L"15" };

enum ChessmanColor
{
    ChessmanColor_None = 0,
    ChessmanColor_Black,
    ChessmanColor_White,
};

inline ChessmanColor operator ! (ChessmanColor color)
{
    static const ChessmanColor colorMap[] =
    {
        /* ChessmanColor_None   => */ ChessmanColor_None,
        /* ChessmanColor_Black  => */ ChessmanColor_White,
        /* ChessmanColor_White  => */ ChessmanColor_Black,
    };
    return colorMap[color];
}

typedef ChessmanColor ChessData[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
typedef double ChessboardValue[CHESSBOARD_SIZE][CHESSBOARD_SIZE];

struct Point
{
    int x;
    int y;

    Point(int x = 0, int y = 0) : x(x), y(y)
    {

    }
};

static const Point INVALID_POSITION = { -1, -1 };

enum Direction
{
    Direction_Right,
    Direction_Down,
    Direction_DownLeft,
    Direction_DownRight,

    Direction_Count,
};

static const Point DirectionDef[Direction_Count] =
{
    {  1, 0 },    // 向右，x += 1（向左的不定义，交换起始点就可以了，下同）
    {  0, 1 },    // 向下，y += 1（向上的不定义）
    { -1, 1 },    // 左下，x -= 1，y += 1（右上的不定义）
    {  1, 1 },    // 右下，x += 1，y += 1（左上的不定义）
};

static const wchar_t * const DIRECTION_TAG[Direction_Count] = { L"─", L"│", L"╱", L"╲" };

struct BlankInfo
{
    int HeadRemain; // 起始端剩余空位数，受阻的话给 0
    int TailRemain; // 结尾端剩余空位数，受阻的话给 0
    int HolePos;    // 中间空位位置，没有的话给 0
};

struct LineInfo
{
    int         Count;      // 个数
    Direction   Direction;  // 方向
    BlankInfo   Blank;      // 空位情况
    Point       Position;   // 起始位置
};

typedef xl::Array<LineInfo> LineInfoCollection;

inline Point operator + (const Point &lhs, const Point &rhs)
{
    Point pt = { lhs.x + rhs.x, lhs.y + rhs.y };
    return pt;
}
inline Point operator - (const Point &lhs, const Point &rhs)
{
    Point pt = { lhs.x - rhs.x, lhs.y - rhs.y };
    return pt;
}
inline Point operator * (const Point &lhs, int rhs)
{
    Point pt = { lhs.x * rhs, lhs.y * rhs};
    return pt;
}
inline bool operator == (const Point &lhs, const Point &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
inline bool operator != (const Point &lhs, const Point &rhs)
{
    return lhs.x != rhs.x || lhs.y != rhs.y;
}


#include "Log.h"

#define LOG_LINE(li)                                                                \
    XL_LOG_INFO(L"%d-line, %s%s, %s, L%dR%dM%d, %d",                                \
                (li).Count,                                                         \
                COORD_TAG_HORZ[(li).Position.x], COORD_TAG_VERT[(li).Position.y],   \
                DIRECTION_TAG[(li).Direction],                                      \
                (li).Blank.HeadRemain, (li).Blank.TailRemain, (li).Blank.HolePos,   \
                Valuation::EvalLine((li)))

#endif // #ifndef __FIVEMODEL_H_1FE257EC_AAA2_43D3_B918_BF9AE7FAE21B_INCLUDED__
