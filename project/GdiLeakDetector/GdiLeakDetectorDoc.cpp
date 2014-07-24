// GdiLeakDetectorDoc.cpp : implementation of the CGdiLeakDetectorDoc class
//

#include "stdafx.h"
#include "GdiLeakDetector.h"

#include "GdiLeakDetectorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGdiLeakDetectorDoc

IMPLEMENT_DYNCREATE(CGdiLeakDetectorDoc, CDocument)

BEGIN_MESSAGE_MAP(CGdiLeakDetectorDoc, CDocument)
	//{{AFX_MSG_MAP(CGdiLeakDetectorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGdiLeakDetectorDoc construction/destruction

CGdiLeakDetectorDoc::CGdiLeakDetectorDoc()
{
	// TODO: add one-time construction code here

}

CGdiLeakDetectorDoc::~CGdiLeakDetectorDoc()
{
}

BOOL CGdiLeakDetectorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGdiLeakDetectorDoc serialization

void CGdiLeakDetectorDoc::Serialize(CArchive& ar)
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
// CGdiLeakDetectorDoc diagnostics

#ifdef _DEBUG
void CGdiLeakDetectorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGdiLeakDetectorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGdiLeakDetectorDoc commands
