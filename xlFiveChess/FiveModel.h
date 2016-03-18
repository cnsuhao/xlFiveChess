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
static const int WIN_LENGTH         = 5;

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

    Direction_CountOrNone,
};

static const Point DirectionDef[] =
{
    {  1, 0 },    // ���ң�x += 1������Ĳ����壬������ʼ��Ϳ����ˣ���ͬ��
    {  0, 1 },    // ���£�y += 1�����ϵĲ����壩
    { -1, 1 },    // ���£�x -= 1��y += 1�����ϵĲ����壩
    {  1, 1 },    // ���£�x += 1��y += 1�����ϵĲ����壩
    {  0, 0 },    // �޷�������һ���ӵ����
};

struct BlockedInfo
{
    int HeadRemain; // ��ʼ��ʣ��հ���������Ļ��� 0
    int TailRemain; // ��β��ʣ��հ���������Ļ��� 0
    int HolePos;    // �м�հ�λ�ã�û�еĻ��� 0
};

struct LineInfo
{
    int         Count;      // ����
    Direction   Direction;  // ����
    BlockedInfo Blocked;    // �������
    Point       Position;   // ��ʼλ��
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

#define LOG_LINE(li)                                                                        \
    XL_LOG_INFO(L"[Line] Pos:(%d,%d), Dir:(%d,%d), Count:%d, Blocked:(%d,%d,%d), Value: %d",\
                (li).Position.x, (li).Position.y,                                           \
                DirectionDef[(li).Direction].x, DirectionDef[(li).Direction].y,             \
                (li).Count,                                                                 \
                (li).Blocked.HeadRemain, (li).Blocked.TailRemain, (li).Blocked.HolePos,     \
                Valuation::EvalLine((li)))

#endif // #ifndef __FIVEMODEL_H_1FE257EC_AAA2_43D3_B918_BF9AE7FAE21B_INCLUDED__
