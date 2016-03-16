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
#include <memory>
#include <time.h>

FiveChess::FiveChess() : m_CurrentTurn(None)
{
    Clear();
}

FiveChess::~FiveChess()
{

}

const FiveChess::ChessData &FiveChess::GetChessData() const
{
    return m_ChessData;
}

FiveChess::ChessmanColor FiveChess::WhoseTurn() const
{
    return m_CurrentTurn;
}

FiveChess::ChessmanColor FiveChess::WhoWins() const
{
    return None;
}

void FiveChess::NewGame()
{
    Clear();
    m_CurrentTurn = Black;
    srand((unsigned int)time(NULL));
}

bool FiveChess::Move(int x, int y, ChessmanColor color)
{
    if (m_CurrentTurn == None || m_CurrentTurn != color)
    {
        return false;
    }

    if (m_ChessData[x][y] != None)
    {
        return false;
    }

    m_ChessData[x][y] = color;
    m_CurrentTurn = color == Black ? White : Black;

    return true;
}

bool FiveChess::AutoMove(ChessmanColor color)
{
    if (m_CurrentTurn == None || m_CurrentTurn != color)
    {
        return false;
    }

    while (true)
    {
        int x = rand() % CHESSBOARD_SIZE;
        int y = rand() % CHESSBOARD_SIZE;

        if (m_ChessData[x][y] == None)
        {
            m_ChessData[x][y] = color;
            m_CurrentTurn = color == Black ? White : Black;
            break;
        }
    }

    return true;
}

void FiveChess::Clear()
{
    memset(&m_ChessData, 0, sizeof(m_ChessData));
}
