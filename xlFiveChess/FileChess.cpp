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

bool FiveChess::IsGameOver() const
{
    return m_Winner != ChessmanColor_None || m_ChessHistory.Size() >= CHESSBOARD_SIZE * CHESSBOARD_SIZE;
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

LineInfoCollection FiveChess::GetPreWinLines(ChessmanColor color) const
{
    LineInfoCollection lic;
    Valuation::FindLine(m_ChessData, CHESS_LENGTH - 2, color, true, true, &lic);

    for (LineInfoCollection::Iterator it = lic.Begin(); it != lic.End(); )
    {
        if (it->Count < CHESS_LENGTH - 1 && (it->Blank.HeadRemain <= 0 || it->Blank.TailRemain <= 0))
        {
            it = lic.Delete(it);
        }
        else
        {
            ++it;
        }
    }

    return lic;
}

void FiveChess::NewGame(ChessmanColor colorFirst)
{
    Clear();
    m_CurrentTurn = colorFirst;
    m_Winner = ChessmanColor_None;
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
    if (m_CurrentTurn == ChessmanColor_None || m_CurrentTurn != color || IsGameOver())
    {
        return false;
    }

    Point pt = INVALID_POSITION;

    if (m_ChessHistory.Empty())
    {
        pt = Policy::FindNextMove(Policy_Center, m_ChessData, color);
    }

    for (int i = 0; i < Policy_Count && pt == INVALID_POSITION; ++i)
    {
        pt = Policy::FindNextMove((PolicyName)i, m_ChessData, color);
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
    m_CurrentTurn = !color;

    CheckGameOver();
}

bool FiveChess::CheckGameOver()
{
    m_Winner = Valuation::FindLine(m_ChessData, CHESS_LENGTH);

    return IsGameOver();
}

void FiveChess::Clear()
{
    memset(m_ChessData, ChessmanColor_None, sizeof(m_ChessData));
    m_ChessHistory.Clear();
}
