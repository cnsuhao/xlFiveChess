// xlFiveChess.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "xlFiveChess.h"

#include "MainFrm.h"
#include "xlFiveChessDoc.h"
#include "xlFiveChessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessApp

BEGIN_MESSAGE_MAP(CxlFiveChessApp, CWinApp)
	//{{AFX_MSG_MAP(CxlFiveChessApp)
	ON_COMMAND(ID_MENU_ABOUT, OnMenuAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessApp construction

CxlFiveChessApp::CxlFiveChessApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CxlFiveChessApp object

CxlFiveChessApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessApp initialization

BOOL CxlFiveChessApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CxlFiveChessDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CxlFiveChessView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
 //   m_pMainWnd->SetWindowPos(&CWnd::wndTop, 
 //                            (GetSystemMetrics(SM_CXFULLSCREEN)-720)/2,
 //                            (GetSystemMetrics(SM_CYFULLSCREEN)-620)/2,
 //                            720, 620, SWP_HIDEWINDOW);
 // 在 CMainFrame::PreCreateWindow 中设置过了

    m_pMainWnd->SetWindowText("溪流五子棋");
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_text;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
    //编辑框内容
	m_text = _T("欢迎使用溪流五子棋！\r\n\r\n\r\n溪流网站: http://www.lqms.org/homepage/kjj/\r\n校内 Blog: http://easybar.8866.org/~streamlet/\r\nE-mail: kejinjin@gmail.com\r\nQQ: 23917666\r\nMSN Messenger: cnkjj@hotmail.com");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_EDIT, m_text);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CxlFiveChessApp::OnMenuAbout()  //弹出关于窗口
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
