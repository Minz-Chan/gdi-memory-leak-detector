// RightView.cpp : implementation file
//

#include "stdafx.h"
#include "gdileakdetector.h"
#include "RightView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRightView dialog
#ifdef _DEBUG

void CRightView::AssertValid() const
{
	CRightView::AssertValid();
}

void CRightView::Dump(CDumpContext& dc) const
{
	CRightView::Dump(dc);
}
#endif //_DEBUG


CRightView::CRightView(CWnd* pParent /*=NULL*/)
	: CDialog(CRightView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRightView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRightView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRightView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRightView, CDialog)
	//{{AFX_MSG_MAP(CRightView)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRightView message handlers
