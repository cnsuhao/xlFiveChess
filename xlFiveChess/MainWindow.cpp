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
#include <xl/Windows/GUI/xlDPI.h>

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
    AppendMsgHandler(WM_WINDOWPOSCHANGING, MsgHandler(this, &MainWindow::OnWindowPosChanging));
    AppendMsgHandler(WM_SIZE, MsgHandler(this, &MainWindow::OnSize));

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
    m_MenuGame.AppendMenu(MF_STRING, MENU_ID_NEW_GAME, L"新游戏(&N)");
    m_MenuGame.AppendMenu(MF_STRING, MENU_ID_UNDO, L"悔棋(&U)");
    m_MenuGame.AppendMenu(MF_SEPARATOR, 0, nullptr);
    m_MenuGame.AppendMenu(MF_STRING, MENU_ID_QUIT, L"退出(&Q)");

    m_MenuRule.CreatePopup();
    m_MenuRule.AppendMenu(MF_STRING | MF_UNCHECKED | MF_GRAYED, MENU_ID_FORBIDDEN, L"禁手(&F)");
    m_MenuRule.AppendMenu(MF_STRING | MF_UNCHECKED | MF_GRAYED, MENU_ID_EXCHANGE, L"三手交换(&E)");
    m_MenuRule.AppendMenu(MF_STRING | MF_UNCHECKED | MF_GRAYED, MENU_ID_DOUBLE_MOVE, L"五手两打(&D)");

    m_MenuBar.Create();
    m_MenuBar.AppendMenu(MF_POPUP, (UINT_PTR)(HMENU)m_MenuGame, L"游戏(&G)");
    m_MenuBar.AppendMenu(MF_POPUP, (UINT_PTR)(HMENU)m_MenuRule, L"规则(&R)");
    m_MenuBar.AppendMenu(MF_STRING, MENU_ID_ABOUT, L"关于(&A)");

    int nStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_CLIPCHILDREN;
    return xl::Windows::Window::Create(nullptr, 0, 0, nWidth, nHeight, nStyle, 0, L"AlphaFiveMainWindow", L"AlphaFive", m_MenuBar);
}

void MainWindow::Relayout()
{
    RECT rc = {};
    GetClientRect(&rc);

    m_ChessBoard.MoveWindow(&rc);
}

LRESULT MainWindow::OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    //HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON_APP));
    //SetIcon(hIcon);
    //SetIcon(hIcon, FALSE);

    m_ChessBoard.Create(hWnd, 0, 0, 0, 0, WS_CHILD | WS_VISIBLE);

    Relayout();

    return 0;
}

LRESULT MainWindow::OnEraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    return TRUE;
}

LRESULT MainWindow::OnWindowPosChanging(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    WINDOWPOS *pPos = (WINDOWPOS *)lParam;
    if ((pPos->flags & SWP_NOSIZE) == 0)
    {
        if (pPos->cx < XL_DPI_X(250))
        {
            pPos->cx = XL_DPI_X(250);
        }
        if (pPos->cy < XL_DPI_Y(250))
        {
            pPos->cy = XL_DPI_Y(250);
        }
    }
    return 0;
}

LRESULT MainWindow::OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    Relayout();
    return 0;
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
