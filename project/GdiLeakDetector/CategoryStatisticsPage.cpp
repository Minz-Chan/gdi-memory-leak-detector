// CategoryStatisticsPage.cpp : implementation file
//

#include "stdafx.h"
#include "gdileakdetector.h"
#include "CategoryStatisticsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCategoryStatisticsPage property page

IMPLEMENT_DYNCREATE(CCategoryStatisticsPage, CPropertyPage)

CCategoryStatisticsPage::CCategoryStatisticsPage() : CPropertyPage(CCategoryStatisticsPage::IDD)
{
	//{{AFX_DATA_INIT(CCategoryStatisticsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCategoryStatisticsPage::~CCategoryStatisticsPage()
{
}

void CCategoryStatisticsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCategoryStatisticsPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCategoryStatisticsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCategoryStatisticsPage)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCategoryStatisticsPage message handlers

void CCategoryStatisticsPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//dc.FillSolidRect(0, 0, 755, 425,RGB(255,255,255));
	// Do not call CPropertyPage::OnPaint() for painting messages

	g_Gdi.SetDraw(&dc,5);

	if( !g_Gdi.GetFindProcess() )
	{
		GetDlgItem(IDC_NOTICESTATIC)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_NOTICESTATIC)->ShowWindow(SW_HIDE);
	}

}

void CCategoryStatisticsPage::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	Invalidate();
	CPropertyPage::OnTimer(nIDEvent);
}

int CCategoryStatisticsPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetTimer(0, 1000, NULL);

	return 0;
}

void CCategoryStatisticsPage::OnCancelMode() 
{
	CPropertyPage::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}


