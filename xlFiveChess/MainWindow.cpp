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

    MENU_ID_FORBIDDEN,
    MENU_ID_EXCHANGE,
    MENU_ID_DOUBLE_MOVE,

    MENU_ID_ABOUT,
};

MainWindow::MainWindow()
{
    AppendMsgHandler(WM_CREATE, MsgHandler(this, &MainWindow::OnCreate));
    AppendMsgHandler(WM_ERASEBKGND, MsgHandler(this, &MainWindow::OnEraseBkgnd));

    AppendMenuCommandMsgHandler(MENU_ID_NEW_GAME, CommandMsgHandler(this, &MainWindow::OnMenuNewGame));
    AppendMenuCommandMsgHandler(MENU_ID_UNDO, CommandMsgHandler(this, &MainWindow::OnMenuUndo));
    AppendMenuCommandMsgHandler(MENU_ID_QUIT, CommandMsgHandler(this, &MainWindow::OnMenuQuit));
    AppendMenuCommandMsgHandler(MENU_ID_FORBIDDEN, CommandMsgHandler(this, &MainWindow::OnMenuForbidden));
    AppendMenuCommandMsgHandler(MENU_ID_EXCHANGE, CommandMsgHandler(this, &MainWindow::OnMenuExchange));
    AppendMenuCommandMsgHandler(MENU_ID_DOUBLE_MOVE, CommandMsgHandler(this, &MainWindow::OnMenuDoubleMove));
    AppendMenuCommandMsgHandler(MENU_ID_ABOUT, CommandMsgHandler(this, &MainWindow::OnMenuAbout));
}

MainWindow::~MainWindow()
{

}

bool MainWindow::Create(int nWidth, int nHeight)
{

    m_MenuGame.CreatePopup();
    m_MenuGame.AppendMenu(MF_POPUP | MF_STRING, MENU_ID_NEW_GAME, L"新游戏(&N)");
    m_MenuGame.AppendMenu(MF_POPUP | MF_STRING, MENU_ID_UNDO, L"悔棋(&U)");
    m_MenuGame.AppendMenu(MF_POPUP | MF_SEPARATOR, 0, nullptr);
    m_MenuGame.AppendMenu(MF_POPUP | MF_STRING, MENU_ID_QUIT, L"退出(&Q)");

    m_MenuRule.CreatePopup();
    m_MenuRule.AppendMenu(MF_POPUP | MF_STRING | MF_UNCHECKED | MF_GRAYED, MENU_ID_FORBIDDEN, L"禁手(&F)");
    m_MenuRule.AppendMenu(MF_POPUP | MF_STRING | MF_UNCHECKED | MF_GRAYED, MENU_ID_EXCHANGE, L"三手交换(&E)");
    m_MenuRule.AppendMenu(MF_POPUP | MF_STRING | MF_UNCHECKED | MF_GRAYED, MENU_ID_DOUBLE_MOVE, L"五手两打(&D)");

    m_MenuBar.Create();
    m_MenuBar.AppendMenu(MF_POPUP, (UINT_PTR)(HMENU)m_MenuGame, L"游戏(&G)");
    m_MenuBar.AppendMenu(MF_POPUP, (UINT_PTR)(HMENU)m_MenuRule, L"规则(&R)");
    m_MenuBar.AppendMenu(MF_STRING, MENU_ID_ABOUT, L"关于(&A)");

    return xl::Windows::Window::Create(nullptr, 0, 0, nWidth, nHeight, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN, 0, L"AlphaFiveMainWindow", L"AlphaFive", m_MenuBar);
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

LRESULT MainWindow::OnMenuForbidden(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled)
{
    return 0;
}

LRESULT MainWindow::OnMenuExchange(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled)
{
    return 0;
}

LRESULT MainWindow::OnMenuDoubleMove(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled)
{
    return 0;
}

LRESULT MainWindow::OnMenuAbout(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled)
{
    MessageBox(L"AlphaFive by Streamlet.", L"关于", MB_OK | MB_ICONINFORMATION);
    return 0;
}
