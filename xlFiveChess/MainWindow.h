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
    LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnEraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnMenuNewGame(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled);
    LRESULT OnMenuUndo(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled);
    LRESULT OnMenuQuit(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled);

private:
    ChessBoard m_ChessBoard;
    xl::Windows::Menu m_MainMenu;
    xl::Windows::Menu m_PopupMenu;
};

#endif // #ifndef __MAINWINDOW_H_4AE40315_5E8F_4AA6_83E4_0532ABFF1647_INCLUDED__
