//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   ChessBoard.cpp
//    Author:      Streamlet
//    Create Time: 2016-03-16
//    Description: 
//
//------------------------------------------------------------------------------


#include "ChessBoard.h"
#include <xl/Windows/GUI/xlDPI.h>
#include <windowsx.h>

ChessBoard::ChessBoard() : m_OperatorColor(ChessmanColor_None), m_nBlockSize(0), m_nLeftBlank(0), m_nTopBlank(0), m_hFont(NULL)
{
    AppendMsgHandler(WM_CREATE, MsgHandler(this, &ChessBoard::OnCreate));
    AppendMsgHandler(WM_ERASEBKGND, MsgHandler(this, &ChessBoard::OnEraseBkgnd));
    AppendMsgHandler(WM_PAINT, MsgHandler(this, &ChessBoard::OnPaint));
    AppendMsgHandler(WM_SIZE, MsgHandler(this, &ChessBoard::OnSize));

    AppendMsgHandler(WM_LBUTTONDOWN, MsgHandler(this, &ChessBoard::OnLButtonDown));
    AppendMsgHandler(WM_LBUTTONUP, MsgHandler(this, &ChessBoard::OnLButtonUp));
    AppendMsgHandler(WM_MOUSEMOVE, MsgHandler(this, &ChessBoard::OnMouseMove));

    ZeroMemory(&m_ptMouseDown, sizeof(m_ptMouseDown));

    m_ptPreMove.x = -1;
    m_ptPreMove.y = -1;

    m_hFont = CreateFont(XL_DPI_Y(-64), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Î¢ÈíÑÅºÚ");
}

ChessBoard::~ChessBoard()
{
    DeleteObject(m_hFont);
}

void ChessBoard::NewGame()
{
    m_OperatorColor = !m_OperatorColor;
    m_FiveChess.NewGame(ChessmanColor_Black);

    if (m_OperatorColor == ChessmanColor_White)
    {
        m_FiveChess.AutoMove(ChessmanColor_Black);
    }

    Invalidate();
}

void ChessBoard::Undo()
{
    if (m_FiveChess.Undo(2))
    {
        Invalidate();
    }
}

void ChessBoard::InitializeCoord()
{
    RECT rc = {};
    GetClientRect(&rc);

    m_nBlockSize = min(rc.right - rc.left, rc.bottom - rc.top) / m_nChessBoardSize;
    int nTotalSize = m_nBlockSize * m_nChessBoardSize;
    m_nLeftBlank = ((rc.right - rc.left) - nTotalSize) / 2;
    m_nTopBlank = ((rc.bottom - rc.top) - nTotalSize) / 2;
}

bool ChessBoard::LogicalToPhysical(POINT &pt)
{
    if (pt.x < 0 || pt.x >= m_nChessBoardSize || pt.y < 0 || pt.y >= m_nChessBoardSize)
    {
        return false;
    }

    pt.x = m_nLeftBlank + pt.x * m_nBlockSize + m_nBlockSize / 2;
    pt.y = m_nTopBlank + pt.y * m_nBlockSize + m_nBlockSize / 2;

    return true;
}

bool ChessBoard::PhysicalToLogical(POINT &pt)
{
    int nTotalSize = m_nBlockSize * m_nChessBoardSize;

    if (pt.x < m_nLeftBlank || pt.x >= m_nLeftBlank + nTotalSize || pt.y < m_nTopBlank || pt.y >= m_nTopBlank + nTotalSize)
    {
        return false;
    }

    pt.x = (pt.x - m_nLeftBlank) / m_nBlockSize;
    pt.y = (pt.y - m_nTopBlank) / m_nBlockSize;

    return true;
}

void ChessBoard::DrawChessBoard(HDC hDC)
{
    HPEN hPen = (HPEN)GetStockObject(BLACK_PEN);
    HPEN hOldPen = SelectPen(hDC, hPen);

    for (int i = 0; i < m_nChessBoardSize; ++i)
    {
        POINT pt1 = { i, 0 }, pt2 = { i, m_nChessBoardSize - 1 };
        LogicalToPhysical(pt1);
        LogicalToPhysical(pt2);
        MoveToEx(hDC, (pt1.x), (pt1.y), NULL);
        LineTo(hDC, (pt2.x), (pt2.y));

        POINT pt3 = { 0, i }, pt4 = { m_nChessBoardSize - 1, i };
        LogicalToPhysical(pt3);
        LogicalToPhysical(pt4);
        MoveToEx(hDC, (pt3.x), (pt3.y), NULL);
        LineTo(hDC, (pt4.x), (pt4.y));
    }

    SelectPen(hDC, hOldPen);
}

void ChessBoard::DrawChessman(HDC hDC, POINT pt, bool bBlack, bool bHiglight, bool bWeak)
{
    LogicalToPhysical(pt);
    int nDelta = m_nBlockSize / 3;

    int nBrushTable[][2] =
    {                  /*  !bWeak       bWeak*/
        /* !bBlack => */ { WHITE_BRUSH, LTGRAY_BRUSH },
        /* bBlack  => */ { BLACK_BRUSH, DKGRAY_BRUSH },
    };

    HBRUSH hBrush = (HBRUSH)GetStockObject(nBrushTable[bBlack][bWeak]);
    HBRUSH hOldBrush = SelectBrush(hDC, hBrush);
    Ellipse(hDC, pt.x - nDelta, pt.y - nDelta, pt.x + nDelta, pt.y + nDelta);

    if (bHiglight)
    {
        nDelta /= 3;
        RECT rc = { pt.x - nDelta, pt.y - nDelta, pt.x + nDelta, pt.y + nDelta };
        SetBkColor(hDC, RGB(0x80, 0x80, 0x80));
        ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
    }

    SelectBrush(hDC, hOldBrush);
}


LRESULT ChessBoard::OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    InitializeCoord();

    m_OperatorColor = ChessmanColor_Black;
    m_FiveChess.NewGame(m_OperatorColor);

    return 0;
}

LRESULT ChessBoard::OnEraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    return TRUE;
}

LRESULT ChessBoard::OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    RECT rc = {};
    GetClientRect(&rc);

    PAINTSTRUCT ps = {};
    BeginPaint(&ps);

    HDC hDC = CreateCompatibleDC(ps.hdc);
    HBITMAP hBmp = CreateCompatibleBitmap(ps.hdc, rc.right - rc.left, rc.bottom - rc.top);
    HBITMAP hOldBmp = SelectBitmap(hDC, hBmp);

    SetBkColor(hDC, RGB(0xff, 0xff, 0xff));
    ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);

    DrawChessBoard(hDC);

    const ChessData &data = m_FiveChess.GetChessData();

    for (int i = 0; i < m_nChessBoardSize; ++i)
    {
        for (int j = 0; j < m_nChessBoardSize; ++j)
        {
            if (data[i][j] != ChessmanColor_None)
            {
                POINT pt = { i, j };
                const FiveChessAction &last = m_FiveChess.GetLastAction();
                bool bHighlight = (pt.x == last.Position.x && pt.y == last.Position.y);
                DrawChessman(hDC, pt, data[i][j] == ChessmanColor_Black, bHighlight);
            }
        }
    }

    if (m_FiveChess.WhoseTurn() == m_OperatorColor && m_ptPreMove.x != -1 && m_ptPreMove.y != -1)
    {
        DrawChessman(hDC, m_ptPreMove, m_OperatorColor == ChessmanColor_Black, false, true);
    }

    if (m_FiveChess.WhoWins() != ChessmanColor_None)
    {
        LPCWSTR lpsz = m_FiveChess.WhoWins() == ChessmanColor_Black ? L"ºÚÆåÊ¤" : L"°×ÆåÊ¤";
        SetBkMode(hDC, TRANSPARENT);
        SelectFont(hDC, m_hFont);
        SetTextColor(hDC, RGB(0xff, 0x00, 0x00));
        DrawText(hDC, lpsz, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }

    BitBlt(ps.hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hDC, 0, 0, SRCCOPY);

    SelectBitmap(hDC, hOldBmp);
    DeleteObject(hBmp);
    DeleteObject(hDC);

    EndPaint(&ps);

    return 0;
}

LRESULT ChessBoard::OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    InitializeCoord();
    return 0;
}

LRESULT ChessBoard::OnLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    SetCapture();

    m_ptMouseDown.x = GET_X_LPARAM(lParam);
    m_ptMouseDown.y = GET_Y_LPARAM(lParam);

    return 0;
}

LRESULT ChessBoard::OnLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    ReleaseCapture();

    if (m_FiveChess.WhoseTurn() != m_OperatorColor)
    {
        return 0;
    }

    if (abs(GET_X_LPARAM(lParam) - m_ptMouseDown.x) > GetSystemMetrics(SM_CXDRAG) ||
        abs(GET_Y_LPARAM(lParam) - m_ptMouseDown.y) > GetSystemMetrics(SM_CYDRAG))
    {
        return 0;
    }

    POINT pt = m_ptMouseDown;

    if (!PhysicalToLogical(pt))
    {
        return 0;
    }

    m_FiveChess.Move(pt.x, pt.y, m_OperatorColor);
    m_FiveChess.AutoMove(!m_OperatorColor);

    Invalidate();

    return 0;
}

LRESULT ChessBoard::OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    if (PhysicalToLogical(pt))
    {
        m_ptPreMove = pt;
    }
    else
    {
        m_ptPreMove.x = -1;
        m_ptPreMove.y = -1;
    }

    Invalidate();

    return 0;
}
