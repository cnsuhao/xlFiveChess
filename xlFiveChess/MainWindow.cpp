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
}

MainWindow::~MainWindow()
{

}

bool MainWindow::Create(int nWidth, int nHeight)
{
    return xl::Windows::Window::Create(nullptr, 0, 0, nWidth, nHeight, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN, 0, L"AlphaFiveMainWindow", L"AlphaFive");
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
