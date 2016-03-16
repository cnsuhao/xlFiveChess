// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "xlFiveChess.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
    ID_SEPARATOR,
    ID_SEPARATOR,
    ID_SEPARATOR,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("创建状态栏失败。\n");
		return -1;      // fail to create
	}

    //设置状态栏
    m_wndStatusBar.SetPaneInfo(0, 0, SBPS_STRETCH, 0);
    m_wndStatusBar.SetPaneInfo(1, 0, SBPS_POPOUT, 180);
    m_wndStatusBar.SetPaneInfo(2, 0, SBPS_POPOUT, 100);
    m_wndStatusBar.SetPaneInfo(3, 0, SBPS_POPOUT, 100);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

    cs.style&=~(WS_MAXIMIZEBOX | WS_SIZEBOX);  //禁止最大化按钮、禁止调节窗口大小
    cs.cx=720;  //设置窗口大小
    cs.cy=620;
    cs.x=(GetSystemMetrics(SM_CXFULLSCREEN)-720)/2;  //置中
    cs.y=(GetSystemMetrics(SM_CYFULLSCREEN)-620)/2;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

