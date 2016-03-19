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

const FiveChessAction &FiveChess::GetLastAction() const
{
    if (!m_ChessHistory.Empty())
    {
        return *m_ChessHistory.ReverseBegin();
    }

    static FiveChessAction dummy;
    return dummy;
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

    Point pt = Policy::FindNextMove(m_ChessData, color);

    if (pt == INVALID_POSITION)
    {
        if (m_ChessHistory.Empty())
        {
            pt.x = CHESSBOARD_SIZE / 2;
            pt.y = CHESSBOARD_SIZE / 2;
        }
        else
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
    }

    ForceMove(pt.x, pt.y, color);

    return true;
}

bool FiveChess::Undo(size_t nSteps)
{
    if (m_ChessHistory.Size() < nSteps)
    {
        return false;
    }

    for (size_t i = 0; i < nSteps; ++i)
    {
        const FiveChessAction &action = *m_ChessHistory.ReverseBegin();
        m_CurrentTurn = action.Color;
        m_ChessData[action.Position.x][action.Position.y] = ChessmanColor_None;
        m_ChessHistory.PopBack();
    }

    m_Winner = ChessmanColor_None;

    return true;
}

void FiveChess::ForceMove(int x, int y, ChessmanColor color)
{
    m_ChessData[x][y] = color;
    m_ChessHistory.PushBack(FiveChessAction(color, Point(x, y)));

    if (IsGameOver())
    {
        m_CurrentTurn = ChessmanColor_None;
    }
    else
    {
        m_CurrentTurn = !color;
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
    m_ChessHistory.Clear();
}
