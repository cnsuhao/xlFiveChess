//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   MainWindow.cpp
//    Author:      Streamlet
//    Create Time: 2016-03-16
//    Description: 
//
//------------------------------------------------------------------------------


#include "MainWindow.h"

enum
{
    MENU_ID_NEW_GAME = 100,
    MENU_ID_UNDO,
    MENU_ID_QUIT,
};

MainWindow::MainWindow()
{
    AppendMsgHandler(WM_CREATE, MsgHandler(this, &MainWindow::OnCreate));
    AppendMsgHandler(WM_ERASEBKGND, MsgHandler(this, &MainWindow::OnEraseBkgnd));

    AppendMenuCommandMsgHandler(MENU_ID_NEW_GAME, CommandMsgHandler(this, &MainWindow::OnMenuNewGame));
    AppendMenuCommandMsgHandler(MENU_ID_UNDO, CommandMsgHandler(this, &MainWindow::OnMenuUndo));
    AppendMenuCommandMsgHandler(MENU_ID_QUIT, CommandMsgHandler(this, &MainWindow::OnMenuQuit));
}

MainWindow::~MainWindow()
{

}

bool MainWindow::Create(int nWidth, int nHeight)
{

    m_PopupMenu.CreatePopup();
    m_PopupMenu.AppendMenu(MF_POPUP | MF_STRING, MENU_ID_NEW_GAME, L"新游戏(&N)");
    m_PopupMenu.AppendMenu(MF_POPUP | MF_STRING, MENU_ID_UNDO, L"悔棋(&U)");
    m_PopupMenu.AppendMenu(MF_POPUP | MF_SEPARATOR, 0, nullptr);
    m_PopupMenu.AppendMenu(MF_POPUP | MF_STRING, MENU_ID_QUIT, L"退出(&Q)");

    m_MainMenu.Create();
    m_MainMenu.AppendMenu(MF_POPUP, (UINT_PTR)(HMENU)m_PopupMenu, L"游戏(&G)");

    return xl::Windows::Window::Create(nullptr, 0, 0, nWidth, nHeight, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN, 0, L"AlphaFiveMainWindow", L"AlphaFive", m_MainMenu);
}

LRESULT MainWindow::OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    //HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON_APP));
    //SetIcon(hIcon);
    //SetIcon(hIcon, FALSE);

    RECT rc = {};
    GetClientRect(&rc);

    m_ChessBoard.Create(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, WS_CHILD | WS_VISIBLE);

    return 0;
}

LRESULT MainWindow::OnEraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    return TRUE;
}

LRESULT MainWindow::OnMenuNewGame(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled)
{
    m_ChessBoard.NewGame();
    return 0;
}

LRESULT MainWindow::OnMenuUndo(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled)
{
    m_ChessBoard.Undo();
    return 0;
}

LRESULT MainWindow::OnMenuQuit(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled)
{
    PostMessage(WM_CLOSE, 0, 0);
    return 0;
}
