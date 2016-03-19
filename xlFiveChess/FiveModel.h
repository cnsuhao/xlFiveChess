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
    {  1, 0 },    // ���ң�x += 1������Ĳ����壬������ʼ��Ϳ����ˣ���ͬ��
    {  0, 1 },    // ���£�y += 1�����ϵĲ����壩
    { -1, 1 },    // ���£�x -= 1��y += 1�����ϵĲ����壩
    {  1, 1 },    // ���£�x += 1��y += 1�����ϵĲ����壩
};

static const wchar_t * const DIRECTION_TAG[Direction_Count] = { L"��", L"��", L"�u", L"�v" };

struct BlankInfo
{
    int HeadRemain; // ��ʼ��ʣ���λ��������Ļ��� 0
    int TailRemain; // ��β��ʣ���λ��������Ļ��� 0
    int HolePos;    // �м��λλ�ã�û�еĻ��� 0
};

struct LineInfo
{
    int         Count;      // ����
    Direction   Direction;  // ����
    BlankInfo   Blank;      // ��λ���
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

#define LOG_LINE(li)                                                                \
    XL_LOG_INFO(L"%d-line, %s%s, %s, L%dR%dM%d, %d",                                \
                (li).Count,                                                         \
                COORD_TAG_HORZ[(li).Position.x], COORD_TAG_VERT[(li).Position.y],   \
                DIRECTION_TAG[(li).Direction],                                      \
                (li).Blank.HeadRemain, (li).Blank.TailRemain, (li).Blank.HolePos,   \
                Valuation::EvalLine((li)))

#endif // #ifndef __FIVEMODEL_H_1FE257EC_AAA2_43D3_B918_BF9AE7FAE21B_INCLUDED__
