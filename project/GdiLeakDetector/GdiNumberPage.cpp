// GdiNumberPage.cpp : implementation file
//

#include "stdafx.h"
#include "gdileakdetector.h"
#include "GdiNumberPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGdiNumberPage property page

IMPLEMENT_DYNCREATE(CGdiNumberPage, CPropertyPage)

CGdiNumberPage::CGdiNumberPage() : CPropertyPage(CGdiNumberPage::IDD)
{
	//{{AFX_DATA_INIT(CGdiNumberPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CGdiNumberPage::~CGdiNumberPage()
{
}

void CGdiNumberPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGdiNumberPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGdiNumberPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGdiNumberPage)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGdiNumberPage message handlers

void CGdiNumberPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	dc.FillSolidRect(0, 0, 755, 425,RGB(255,255,255));
	g_Gdi.SetDraw(&dc,3);

	if( !g_Gdi.GetFindProcess() )
	{
		GetDlgItem(IDC_NOTICESTATIC)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_NOTICESTATIC)->ShowWindow(SW_HIDE);
	}

}

void CGdiNumberPage::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate();
	CPropertyPage::OnTimer(nIDEvent);
}

int CGdiNumberPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetTimer(3, 1000, NULL);
	return 0;
}

