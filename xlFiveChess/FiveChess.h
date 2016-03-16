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


class FiveChess
{
public:
    static const int CHESSBOARD_SIZE = 15;

    enum ChessmanColor
    {
        None = 0,
        Black,
        White,
    };

    typedef ChessmanColor ChessData[CHESSBOARD_SIZE][CHESSBOARD_SIZE];

public:
    FiveChess();
    ~FiveChess();

public:
    const ChessData &GetChessData() const;
    ChessmanColor WhoseTurn() const;
    ChessmanColor WhoWins() const;

public:
    void NewGame(ChessmanColor colorFirst);
    bool Move(int x, int y, ChessmanColor color);
    bool AutoMove(ChessmanColor color);

private:
    void ForceMove(int x, int y, ChessmanColor color);
    bool IsGameOver();
    void Clear();

private:
    ChessmanColor m_CurrentTurn;
    ChessmanColor m_Winner;
    ChessData m_ChessData;
};

#endif // #ifndef __FIVECHESS_H_DD3430E6_BC82_4E36_9D4F_99A5262E2B5B_INCLUDED__
