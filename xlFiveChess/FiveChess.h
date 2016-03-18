//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   FiveChess.h
//    Author:      Streamlet
//    Create Time: 2016-03-16
//    Description: 
//
//------------------------------------------------------------------------------

#ifndef __FIVECHESS_H_DD3430E6_BC82_4E36_9D4F_99A5262E2B5B_INCLUDED__
#define __FIVECHESS_H_DD3430E6_BC82_4E36_9D4F_99A5262E2B5B_INCLUDED__


#include "FiveModel.h"
#include <xl/Common/Containers/xlArray.h>

struct FiveChessAction
{
    ChessmanColor Color;
    Point Position;

    FiveChessAction(ChessmanColor color = ChessmanColor_None, Point position = INVALID_POSITION) : Color(color), Position(position)
    {

    }
};

typedef xl::Deque<FiveChessAction> FiveChessHistory;

class FiveChess
{
public:
    FiveChess();
    ~FiveChess();

public:
    const ChessData &GetChessData() const;
    ChessmanColor WhoseTurn() const;
    ChessmanColor WhoWins() const;
    const FiveChessAction &GetLastAction() const;

public:
    void NewGame(ChessmanColor colorFirst);
    bool Move(int x, int y, ChessmanColor color);
    bool AutoMove(ChessmanColor color);
    bool Undo();

private:
    void ForceMove(int x, int y, ChessmanColor color);
    bool IsGameOver();
    void Clear();

private:
    ChessmanColor m_CurrentTurn;
    ChessmanColor m_Winner;
    ChessData m_ChessData;
    FiveChessHistory m_ChessHistory;
};

#endif // #ifndef __FIVECHESS_H_DD3430E6_BC82_4E36_9D4F_99A5262E2B5B_INCLUDED__
