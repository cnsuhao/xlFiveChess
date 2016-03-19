//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   MainWindow.h
//    Author:      Streamlet
//    Create Time: 2016-03-16
//    Description: 
//
//------------------------------------------------------------------------------

#ifndef __MAINWINDOW_H_4AE40315_5E8F_4AA6_83E4_0532ABFF1647_INCLUDED__
#define __MAINWINDOW_H_4AE40315_5E8F_4AA6_83E4_0532ABFF1647_INCLUDED__

#include <xl/Windows/GUI/xlWindow.h>
#include <xl/Windows/GUI/xlMenu.h>
#include "ChessBoard.h"

class MainWindow : public xl::Windows::Window
{
public:
    MainWindow();
    ~MainWindow();

    bool Create(int nWidth, int nHeight);

private:
    void Relayout();

private:
    LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnEraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnWindowPosChanging(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnMenuNewGame(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled);
    LRESULT OnMenuUndo(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled);
    LRESULT OnMenuQuit(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled);
    LRESULT OnMenuForbidden(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled);
    LRESULT OnMenuExchange(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled);
    LRESULT OnMenuDoubleMove(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled);
    LRESULT OnMenuAbout(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled);

private:
    ChessBoard m_ChessBoard;
    xl::Windows::Menu m_MenuBar;
    xl::Windows::Menu m_MenuGame;
    xl::Windows::Menu m_MenuRule;
};

#endif // #ifndef __MAINWINDOW_H_4AE40315_5E8F_4AA6_83E4_0532ABFF1647_INCLUDED__
