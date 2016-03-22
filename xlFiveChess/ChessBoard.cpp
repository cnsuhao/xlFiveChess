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
#include <stdio.h>
#include "Valuation.h"

#define COORD_FONT_SIZE 12

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

    m_hFont = CreateFont(XL_DPI_Y(-COORD_FONT_SIZE), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"微软雅黑");
    m_hFontBig = CreateFont(XL_DPI_Y(-64), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"微软雅黑");

#ifdef DRAW_DEBUG_INFO
    m_hFontDebug = CreateFont(XL_DPI_Y(-8), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"微软雅黑");
    m_hPenDebug = CreatePen(PS_SOLID, 1, RGB(0xff, 0x00, 0x00));
#endif
}

ChessBoard::~ChessBoard()
{
#ifdef DRAW_DEBUG_INFO
    DeleteObject(m_hPenDebug);
    DeleteObject(m_hFont);
#endif
    DeleteObject(m_hFontBig);
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

#ifdef DRAW_DEBUG_INFO
    Policy::ClearLastChessboardValues();
    Policy::ClearLastLineInfoCollection();
#endif

    Invalidate();
}

void ChessBoard::Undo()
{
    if (m_FiveChess.Undo(m_FiveChess.WhoseTurn() == m_OperatorColor ? 2 : 1))
    {
        Invalidate();
    }
}

void ChessBoard::InitializeCoord()
{
    RECT rc = {};
    GetClientRect(&rc);

    m_nBlockSize = min(rc.right - rc.left - COORD_FONT_SIZE * 2, rc.bottom - rc.top - COORD_FONT_SIZE) / CHESSBOARD_SIZE;
    int nTotalSize = m_nBlockSize * CHESSBOARD_SIZE;
    m_nLeftBlank = ((rc.right - rc.left) - nTotalSize) / 2 + COORD_FONT_SIZE;
    m_nTopBlank = ((rc.bottom - rc.top) - nTotalSize) / 2 + COORD_FONT_SIZE;
}

bool ChessBoard::LogicalToPhysical(POINT &pt)
{
    if (pt.x < 0 || pt.x >= CHESSBOARD_SIZE || pt.y < 0 || pt.y >= CHESSBOARD_SIZE)
    {
        return false;
    }

    pt.x = m_nLeftBlank + pt.x * m_nBlockSize + m_nBlockSize / 2;
    pt.y = m_nTopBlank + pt.y * m_nBlockSize + m_nBlockSize / 2;

    return true;
}

bool ChessBoard::PhysicalToLogical(POINT &pt)
{
    int nTotalSize = m_nBlockSize * CHESSBOARD_SIZE;

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
    static const wchar_t * const COORD_TAG_HORZ[CHESSBOARD_SIZE] = { L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O" };
    static const wchar_t * const COORD_TAG_VERT[CHESSBOARD_SIZE] = { L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"11", L"12", L"13", L"14", L"15" };

    HPEN hPen = (HPEN)GetStockObject(BLACK_PEN);
    HPEN hOldPen = SelectPen(hDC, hPen);
    HFONT hOldFont = SelectFont(hDC, m_hFont);
    int nOldBkMode = SetBkMode(hDC, TRANSPARENT);
    COLORREF nOldTextColor = SetTextColor(hDC, RGB(0x00, 0x00, 0x00));

    for (int i = 0; i < CHESSBOARD_SIZE; ++i)
    {
        // 画竖线
        POINT pt1 = { i, 0 }, pt2 = { i, CHESSBOARD_SIZE - 1 };
        LogicalToPhysical(pt1);
        LogicalToPhysical(pt2);
        MoveToEx(hDC, (pt1.x), (pt1.y), NULL);
        LineTo(hDC, (pt2.x), (pt2.y));

        // 画横线
        POINT pt3 = { 0, i }, pt4 = { CHESSBOARD_SIZE - 1, i };
        LogicalToPhysical(pt3);
        LogicalToPhysical(pt4);
        MoveToEx(hDC, (pt3.x), (pt3.y), NULL);
        LineTo(hDC, (pt4.x), (pt4.y));

        // 画横坐标
        RECT rc1 = { 0, 0, pt1.x * 2, pt1.y - XL_DPI_Y(COORD_FONT_SIZE / 2) };
        DrawText(hDC, COORD_TAG_HORZ[i], -1, &rc1, DT_SINGLELINE | DT_CENTER | DT_BOTTOM);
        // 画纵坐标
        RECT rc2 = { 0, 0, pt3.x - XL_DPI_X(COORD_FONT_SIZE), pt3.y * 2 };
        DrawText(hDC, COORD_TAG_VERT[i], -1, &rc2, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
    }

    // 中心点打个标记
    POINT pt = { CHESSBOARD_SIZE / 2, CHESSBOARD_SIZE / 2 };
    DrawHint(hDC, pt, RGB(0x00, 0x00, 0x00));

    SetTextColor(hDC, nOldTextColor);
    SetBkMode(hDC, nOldBkMode);
    SelectFont(hDC, hOldFont);
    SelectPen(hDC, hOldPen);
}

void ChessBoard::DrawChessman(HDC hDC, POINT pt, bool bBlack, bool bWeak)
{
    LogicalToPhysical(pt);
    int nDelta = m_nBlockSize / 3;

    int nBrushTable[][2] =
    {                  /*  !bWeak       bWeak         */
        /* !bBlack => */ { WHITE_BRUSH, LTGRAY_BRUSH },
        /* bBlack  => */ { BLACK_BRUSH, DKGRAY_BRUSH },
    };

    HBRUSH hBrush = (HBRUSH)GetStockObject(nBrushTable[bBlack][bWeak]);
    HBRUSH hOldBrush = SelectBrush(hDC, hBrush);
    Ellipse(hDC, pt.x - nDelta, pt.y - nDelta, pt.x + nDelta, pt.y + nDelta);

    SelectBrush(hDC, hOldBrush);
}

void ChessBoard::DrawHint(HDC hDC, POINT pt, COLORREF color)
{
    LogicalToPhysical(pt);
    RECT rc = { pt.x - m_nBlockSize / 8, pt.y - m_nBlockSize / 8, pt.x + m_nBlockSize / 8, pt.y + m_nBlockSize / 8 };
    COLORREF nOldBkColor = SetBkColor(hDC, color);
    ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
    SetBkColor(hDC, nOldBkColor);
}

#ifdef DRAW_DEBUG_INFO

void ChessBoard::DrawDebugInfo(HDC hDC)
{
    HPEN hOldPen = SelectPen(hDC, m_hPenDebug);
    HFONT hOldFont = SelectFont(hDC, m_hFontDebug);
    int nOldBkMode = SetBkMode(hDC, TRANSPARENT);
    COLORREF nOldTextColor = SetTextColor(hDC, RGB(0x00, 0x80, 0x00));
    WCHAR szValue[64] = {};

    // 画每个格子的分值

    const ChessboardValue &values = Policy::GetLastChessboardValues();
    const ChessData &data = m_FiveChess.GetChessData();

    for (int i = 0; i < CHESSBOARD_SIZE; ++i)
    {
        for (int j = 0; j < CHESSBOARD_SIZE; ++j)
        {
            POINT pt = { i, j };
            LogicalToPhysical(pt);
            RECT rc = { pt.x, pt.y, pt.x + m_nBlockSize, pt.y + m_nBlockSize };

            swprintf_s(szValue, L"%.1g", values[i][j]);
            DrawText(hDC, szValue, -1, &rc, DT_SINGLELINE | DT_LEFT | DT_TOP);
        }
    }

    SetTextColor(hDC, RGB(0xff, 0x00, 0x00));
    const LineInfoCollection &lic = Policy::GetLastLineInfoCollection();

    for (LineInfoCollection::Iterator it = lic.Begin(); it != lic.End(); ++it)
    {
        if (it->Count < 2)
        {
            continue;
        }

        double dValue = Valuation::EvalLine(*it);

        Point point1 = it->Position, point2 = it->Position + DirectionDef[it->Direction] * (it->Count - 1 + (it->Blank.HolePos > 0 ? 1 : 0));
        POINT pt1 = { point1.x, point1.y }, pt2 = { point2.x, point2.y };

        LogicalToPhysical(pt1);
        LogicalToPhysical(pt2);

        pt1.x -= DirectionDef[it->Direction].x * m_nBlockSize / 3;
        pt1.y -= DirectionDef[it->Direction].y * m_nBlockSize / 3;
        pt2.x += DirectionDef[it->Direction].x * m_nBlockSize / 3;
        pt2.y += DirectionDef[it->Direction].y * m_nBlockSize / 3;

        pt1.x += 1;
        pt1.y += 1;
        pt2.x += 1;
        pt2.y += 1;

        MoveToEx(hDC, (pt1.x), (pt1.y), NULL);
        LineTo(hDC, (pt2.x), (pt2.y));
        RECT rc = { pt2.x, pt2.y, pt2.x + m_nBlockSize, pt2.y + m_nBlockSize };

        swprintf_s(szValue, L"%.1g", dValue);
        DrawText(hDC, szValue, -1, &rc, DT_SINGLELINE | DT_LEFT | DT_TOP);
    }

    SetTextColor(hDC, nOldTextColor);
    SetBkMode(hDC, nOldBkMode);
    SelectFont(hDC, hOldFont);
    SelectPen(hDC, hOldPen);
}

#endif

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

    COLORREF nOldBkColor = SetBkColor(hDC, RGB(0xff, 0xff, 0xff));
    ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
    SetBkColor(hDC, nOldBkColor);

    DrawChessBoard(hDC);

    const ChessData &data = m_FiveChess.GetChessData();

    for (int i = 0; i < CHESSBOARD_SIZE; ++i)
    {
        for (int j = 0; j < CHESSBOARD_SIZE; ++j)
        {
            if (data[i][j] != ChessmanColor_None)
            {
                POINT pt = { i, j };
                DrawChessman(hDC, pt, data[i][j] == ChessmanColor_Black);

                const FiveChessAction &last = m_FiveChess.GetLastAction();

                if (pt.x == last.Position.x && pt.y == last.Position.y)
                {
                    DrawHint(hDC, pt, RGB(0x80, 0x80, 0x80));
                }
            }
        }
    }

    if (m_FiveChess.WhoseTurn() == m_OperatorColor && m_ptPreMove.x != -1 && m_ptPreMove.y != -1)
    {
        DrawChessman(hDC, m_ptPreMove, m_OperatorColor == ChessmanColor_Black, true);
    }

    if (m_FiveChess.IsGameOver())
    {
        LPCWSTR lpsz[] =
        {
            /* ChessmanColor_None  => */ L"平局",
            /* ChessmanColor_Black => */ L"黑棋胜",
            /* ChessmanColor_White => */ L"白棋胜",
        };

        int nOldBkMode = SetBkMode(hDC, TRANSPARENT);
        HFONT hOldFont = SelectFont(hDC, m_hFontBig);
        COLORREF nOldTextColor = SetTextColor(hDC, RGB(0xff, 0x00, 0x00));

        DrawText(hDC, lpsz[m_FiveChess.WhoWins()], -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        SetTextColor(hDC, nOldTextColor);
        SelectFont(hDC, hOldFont);
        SetBkMode(hDC, nOldBkMode);
    }
    else
    {
        const LineInfoCollection &preWin = m_FiveChess.GetPreWinLines();
        COLORREF colors[] =
        {
            /* ChessmanColor_None  => */ RGB(0x00, 0x00, 0x00),
            /* ChessmanColor_Black => */ RGB(0x40, 0x40, 0x40),
            /* ChessmanColor_White => */ RGB(0xc0, 0xc0, 0xc0),
        };

        for (LineInfoCollection::Iterator it = preWin.Begin(); it != preWin.End(); ++it)
        {
            if (it->Blank.HeadRemain > 0 && !(it->Count == CHESS_LENGTH - 1 && it->Blank.HolePos > 0))
            {
                Point point = it->Position - DirectionDef[it->Direction];
                POINT pt = { point.x, point.y };
                LogicalToPhysical(pt);
                DrawHint(hDC, pt, colors[it->Color]);
            }
            if (it->Blank.TailRemain > 0 && !(it->Count == CHESS_LENGTH - 1 && it->Blank.HolePos > 0))
            {
                Point point = it->Position + DirectionDef[it->Direction] * (it->Count + (it->Blank.HolePos > 0 ? 1 : 0));
                POINT pt = { point.x, point.y };
                LogicalToPhysical(pt);
                DrawHint(hDC, pt, colors[it->Color]);
            }
            if (it->Blank.HolePos > 0)
            {
                Point point = it->Position + DirectionDef[it->Direction] * it->Blank.HolePos;
                POINT pt = { point.x, point.y };
                LogicalToPhysical(pt);
                DrawHint(hDC, pt, colors[it->Color]);
            }
        }
    }

#ifdef DRAW_DEBUG_INFO
    DrawDebugInfo(hDC);
#endif

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
    Invalidate();
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
