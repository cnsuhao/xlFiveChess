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

static const int CHESSBOARD_SIZE = 15;

enum ChessmanColor
{
    ChessmanColor_None = 0,
    ChessmanColor_Black,
    ChessmanColor_White,
};

typedef ChessmanColor ChessData[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
typedef double ChessboardValue[CHESSBOARD_SIZE][CHESSBOARD_SIZE];

struct Point
{
    int x;
    int y;
};

static const Point INVALID_POSITION = { -1, -1 };

enum Direction
{
    Direction_Right,
    Direction_Down,
    Direction_DownLeft,
    Direction_DownRight,

    Direction_CountOrNone,
};

static const Point DirectionDef[] =
{
    {  1, 0 },    // 向右，x += 1（向左的不定义，交换起始点就可以了，下同）
    {  0, 1 },    // 向下，y += 1（向上的不定义）
    { -1, 1 },    // 左下，x -= 1，y += 1（右上的不定义）
    {  1, 1 },    // 右下，x += 1，y += 1（坐上的不定义）
    {  0, 0 },    // 无方向，用于一个子的情况
};

struct BlockedInfo
{
    bool HeadBlocked;   // 起始端受阻
    bool TailBlocked;   // 结尾端受阻
};

struct LineInfo
{
    int         Count;      // 个数
    Direction   Direction;  // 方向
    BlockedInfo Blocked;    // 受阻情况
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

#define LOG_LINE(li)                                                            \
    XL_LOG_INFO(L"[Line] Pos:(%d,%d), Dir:(%d,%d), Blocked:(%d,%d), Count:%d",  \
                (li).Position.x, (li).Position.y,                               \
                DirectionDef[(li).Direction].x, DirectionDef[(li).Direction].y, \
                (li).Blocked.HeadBlocked, (li).Blocked.TailBlocked,             \
                (li).Count)

#endif // #ifndef __FIVEMODEL_H_1FE257EC_AAA2_43D3_B918_BF9AE7FAE21B_INCLUDED__
