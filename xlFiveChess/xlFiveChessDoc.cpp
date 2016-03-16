// xlFiveChessDoc.cpp : implementation of the CxlFiveChessDoc class
//

#include "stdafx.h"
#include "xlFiveChess.h"

#include "xlFiveChessDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessDoc

IMPLEMENT_DYNCREATE(CxlFiveChessDoc, CDocument)

BEGIN_MESSAGE_MAP(CxlFiveChessDoc, CDocument)
	//{{AFX_MSG_MAP(CxlFiveChessDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessDoc construction/destruction

CxlFiveChessDoc::CxlFiveChessDoc()
{
	// TODO: add one-time construction code here

}

CxlFiveChessDoc::~CxlFiveChessDoc()
{
}

BOOL CxlFiveChessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessDoc serialization

void CxlFiveChessDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessDoc diagnostics

#ifdef _DEBUG
void CxlFiveChessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CxlFiveChessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CxlFiveChessDoc commands
