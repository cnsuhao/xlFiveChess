//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   FileChess.cpp
//    Author:      Streamlet
//    Create Time: 2016-03-16
//    Description: 
//
//------------------------------------------------------------------------------


#include "FiveChess.h"
#include "Valuation.h"
#include "Policy.h"
#include <memory>
#include <time.h>

FiveChess::FiveChess() : m_CurrentTurn(ChessmanColor_None), m_Winner(ChessmanColor_None)
{
    Clear();
}

FiveChess::~FiveChess()
{

}

const ChessData &FiveChess::GetChessData() const
{
    return m_ChessData;
}

ChessmanColor FiveChess::WhoseTurn() const
{
    return m_CurrentTurn;
}

ChessmanColor FiveChess::WhoWins() const
{
    return m_Winner;
}

void FiveChess::NewGame(ChessmanColor colorFirst)
{
    Clear();
    m_CurrentTurn = colorFirst;
    m_Winner = ChessmanColor_None;
    srand((unsigned int)time(NULL));
}

bool FiveChess::Move(int x, int y, ChessmanColor color)
{
    if (m_CurrentTurn == ChessmanColor_None || m_CurrentTurn != color)
    {
        return false;
    }

    if (m_ChessData[x][y] != ChessmanColor_None)
    {
        return false;
    }

    ForceMove(x, y, color);

    return true;
}

bool FiveChess::AutoMove(ChessmanColor color)
{
    if (m_CurrentTurn == ChessmanColor_None || m_CurrentTurn != color)
    {
        return false;
    }

    LineInfoCollection ours, theirs;
    Valuation::FindLine(m_ChessData, 1, color, true, &ours);
    Valuation::FindLine(m_ChessData, 1, color == ChessmanColor_Black ? ChessmanColor_White : ChessmanColor_Black, true, &theirs);
    Point pt = Policy::FindNextMove(ours, theirs);

    if (pt == INVALID_POSITION)
    {
        while (true)
        {
            pt.x = rand() % CHESSBOARD_SIZE;
            pt.y = rand() % CHESSBOARD_SIZE;

            if (m_ChessData[pt.x][pt.y] == ChessmanColor_None)
            {
                break;
            }
        }
    }

    ForceMove(pt.x, pt.y, color);

    return true;
}

void FiveChess::ForceMove(int x, int y, ChessmanColor color)
{
    m_ChessData[x][y] = color;

    if (IsGameOver())
    {
        m_CurrentTurn = ChessmanColor_None;
    }
    else
    {
        m_CurrentTurn = color == ChessmanColor_Black ? ChessmanColor_White : ChessmanColor_Black;
    }
}

bool FiveChess::IsGameOver()
{
    m_Winner = Valuation::FindLine(m_ChessData, 5);
    return m_Winner != ChessmanColor_None;
}

void FiveChess::Clear()
{
    memset(&m_ChessData, 0, sizeof(m_ChessData));
}
