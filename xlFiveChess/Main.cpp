//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   Main.cpp
//    Author:      Streamlet
//    Create Time: 2016-03-16
//    Description: 
//
//------------------------------------------------------------------------------



#include <Windows.h>
#include <xl/Windows/GUI/xlDPI.h>
#include "MainWindow.h"
#include "Log.h"


int APIENTRY wWinMain(_In_ HINSTANCE     hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR        lpCmdLine,
    _In_ int           nCmdShow)
{
    MainWindow wnd;

    wnd.AppendMsgHandler(WM_DESTROY, [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) -> LRESULT
    {
        PostQuitMessage(0);
        return FALSE;
    });

    if (!wnd.Create(XL_DPI_X(600), XL_DPI_Y(640)))
    {
        MessageBox(nullptr, L"≥Ã–Ú≥ı ºªØ¥ÌŒÛ°£", L"¥ÌŒÛ", MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }

    wnd.CenterWindow();
    wnd.UpdateWindow();
    wnd.ShowWindow(nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}
