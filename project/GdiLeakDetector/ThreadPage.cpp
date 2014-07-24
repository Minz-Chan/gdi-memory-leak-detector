// ThreadPage.cpp : implementation file
//

#include "stdafx.h"
#include "gdileakdetector.h"
#include "ThreadPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThreadPage property page

IMPLEMENT_DYNCREATE(CThreadPage, CPropertyPage)

CThreadPage::CThreadPage() : CPropertyPage(CThreadPage::IDD)
{
	//{{AFX_DATA_INIT(CThreadPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CThreadPage::~CThreadPage()
{
}

void CThreadPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThreadPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThreadPage, CPropertyPage)
	//{{AFX_MSG_MAP(CThreadPage)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThreadPage message handlers

void CThreadPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
//	dc.FillSolidRect(0, 0, 755, 425,RGB(255,255,255));
	// Do not call CPropertyPage::OnPaint() for painting messages


	g_Gdi.SetDraw(&dc,4);

	if( !g_Gdi.GetFindProcess() )
	{
		GetDlgItem(IDC_NOTICESTATIC)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_NOTICESTATIC)->ShowWindow(SW_HIDE);
	}


}

void CThreadPage::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate();
	CPropertyPage::OnTimer(nIDEvent);

}

int CThreadPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetTimer(4,1000,NULL);
	return 0;
}


