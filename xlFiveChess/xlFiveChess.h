// xlFiveChess.h : main header file for the xlFiveChess application
//

#if !defined(AFX_xlFiveChess_H__1ECCBD48_CF88_409E_A38D_47F39F45BE88__INCLUDED_)
#define AFX_xlFiveChess_H__1ECCBD48_CF88_409E_A38D_47F39F45BE88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessApp:
// See xlFiveChess.cpp for the implementation of this class
//

class CxlFiveChessApp : public CWinApp
{
public:
	CxlFiveChessApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxlFiveChessApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CxlFiveChessApp)
	afx_msg void OnMenuAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_xlFiveChess_H__1ECCBD48_CF88_409E_A38D_47F39F45BE88__INCLUDED_)
