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

MainWindow::MainWindow()
{
    AppendMsgHandler(WM_CREATE, MsgHandler(this, &MainWindow::OnCreate));
    AppendMsgHandler(WM_ERASEBKGND, MsgHandler(this, &MainWindow::OnEraseBkgnd));
    //AppendMsgHandler(WM_WINDOWPOSCHANGING, MsgHandler(this, &MainWindow::OnWindowPosChanging));
    //AppendMsgHandler(WM_SIZE, MsgHandler(this, &MainWindow::OnSize));
}

MainWindow::~MainWindow()
{

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

LRESULT MainWindow::OnWindowPosChanging(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    return 0;
}

LRESULT MainWindow::OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    return 0;
}
