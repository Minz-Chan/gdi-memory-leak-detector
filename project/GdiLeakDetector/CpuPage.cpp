// CpuPage.cpp : implementation file
//

#include "stdafx.h"
#include "gdileakdetector.h"
#include "CpuPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCpuPage property page


IMPLEMENT_DYNCREATE(CCpuPage, CPropertyPage)

CCpuPage::CCpuPage() : CPropertyPage(CCpuPage::IDD)
{
	//{{AFX_DATA_INIT(CCpuPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCpuPage::~CCpuPage()
{
}

void CCpuPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCpuPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCpuPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCpuPage)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCpuPage message handlers

void CCpuPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//	dc.FillSolidRect(0, 0, 755, 425,RGB(255,255,255));
	// Do not call CPropertyPage::OnPaint() for painting messages
	
//	CDC *pDC = GetDC();
	g_Gdi.SetDraw(&dc,1);



}

void CCpuPage::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	Invalidate();
	CPropertyPage::OnTimer(nIDEvent);
}

int CCpuPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetTimer(1,1000,NULL);
	return 0;
}
