// xlFiveChessDoc.h : interface of the CxlFiveChessDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_xlFiveChessDOC_H__9B0D1A4D_52CD_40A6_A01D_9F7662E960CD__INCLUDED_)
#define AFX_xlFiveChessDOC_H__9B0D1A4D_52CD_40A6_A01D_9F7662E960CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CxlFiveChessDoc : public CDocument
{
protected: // create from serialization only
	CxlFiveChessDoc();
	DECLARE_DYNCREATE(CxlFiveChessDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxlFiveChessDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CxlFiveChessDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CxlFiveChessDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_xlFiveChessDOC_H__9B0D1A4D_52CD_40A6_A01D_9F7662E960CD__INCLUDED_)
