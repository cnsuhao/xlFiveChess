//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   ChessBoard.h
//    Author:      Streamlet
//    Create Time: 2016-03-16
//    Description: 
//
//------------------------------------------------------------------------------

#ifndef __CHESSBOARD_H_1C59B29F_E989_4420_8CCA_202D5900AFB7_INCLUDED__
#define __CHESSBOARD_H_1C59B29F_E989_4420_8CCA_202D5900AFB7_INCLUDED__


#include <xl/Windows/GUI/xlWindow.h>
#include "FiveChess.h"

class ChessBoard : public xl::Windows::Window
{
public:
    ChessBoard();
    ~ChessBoard();

    void NewGame();
    void Undo();

private:
    void InitializeCoord();
    bool LogicalToPhysical(POINT &pt);
    bool PhysicalToLogical(POINT &pt);
    void DrawChessBoard(HDC hDC);
    void DrawChessMan(HDC hDC, POINT pt, bool bBlack, bool bHiglight = false, bool bWeak = false);

private:
    LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnEraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

private:
    const static int m_nChessBoardSize = CHESSBOARD_SIZE;

    FiveChess m_FiveChess;
    ChessmanColor m_OperatorColor;;

    int m_nBlockSize;
    int m_nLeftBlank;
    int m_nTopBlank;

    POINT m_ptMouseDown;
    POINT m_ptPreMove;

    HFONT m_hFont;
};


#endif // #ifndef __CHESSBOARD_H_1C59B29F_E989_4420_8CCA_202D5900AFB7_INCLUDED__
