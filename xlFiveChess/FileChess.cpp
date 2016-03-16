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

FiveChess::FiveChess() : m_CurrentTurn(None), m_Winner(None)
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
    return m_Winner;
}

void FiveChess::NewGame(ChessmanColor colorFirst)
{
    Clear();
    m_CurrentTurn = colorFirst;
    m_Winner = None;
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

    ForceMove(x, y, color);

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
            ForceMove(x, y, color);
            break;
        }
    }

    return true;
}

void FiveChess::ForceMove(int x, int y, ChessmanColor color)
{
    m_ChessData[x][y] = color;

    if (IsGameOver())
    {
        m_CurrentTurn = None;
    }
    else
    {
        m_CurrentTurn = color == Black ? White : Black;
    }
}

bool FiveChess::IsGameOver()
{
    const int nDelta[][2] =
    {
        { 1, 0 },    // 向右
        { 0, 1 },    // 向下
        { 1, 1 },    // 向右下
        { -1, 1 },   // 坐下
    };

    for (int i = 0; i < CHESSBOARD_SIZE; ++i)
    {
        for (int j = 0; j < CHESSBOARD_SIZE; ++j)
        {
            if (m_ChessData[i][j] == None)
            {
                continue;
            }

            for (int k = 0; k < _countof(nDelta); ++k)
            {
                if (i + nDelta[k][0] * 4 >= CHESSBOARD_SIZE ||
                    j + nDelta[k][1] * 4 >= CHESSBOARD_SIZE)
                {
                    continue;
                }

                bool bWin = true;

                for (int l = 1; l < 5; ++l)
                {
                    if (m_ChessData[i + nDelta[k][0] * l][j + nDelta[k][1] * l] != m_ChessData[i][j])
                    {
                        bWin = false;
                        break;
                    }
                }

                if (bWin)
                {
                    m_Winner = m_ChessData[i][j];
                    return true;
                }
            }
        }
    }

    return false;
}

void FiveChess::Clear()
{
    memset(&m_ChessData, 0, sizeof(m_ChessData));
}
