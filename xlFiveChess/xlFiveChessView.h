// xlFiveChessView.h : interface of the CxlFiveChessView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_xlFiveChessVIEW_H__189B0834_D2D4_46EC_B3EB_A493D96DE24F__INCLUDED_)
#define AFX_xlFiveChessVIEW_H__189B0834_D2D4_46EC_B3EB_A493D96DE24F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CxlFiveChessView : public CView
{
protected: // create from serialization only
	CxlFiveChessView();
	DECLARE_DYNCREATE(CxlFiveChessView)

// Attributes
public:
	CxlFiveChessDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxlFiveChessView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CxlFiveChessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CxlFiveChessView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMenuStart();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMenuExit();
	afx_msg void OnMenuSound();
	afx_msg void OnUpdateMenuSound(CCmdUI* pCmdUI);
	afx_msg void OnMenuHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in xlFiveChessView.cpp
inline CxlFiveChessDoc* CxlFiveChessView::GetDocument()
   { return (CxlFiveChessDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_xlFiveChessVIEW_H__189B0834_D2D4_46EC_B3EB_A493D96DE24F__INCLUDED_)
