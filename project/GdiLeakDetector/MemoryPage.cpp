// MemoryPage.cpp : implementation file
//

#include "stdafx.h"
#include "gdileakdetector.h"
#include "MemoryPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMemoryPage property page

IMPLEMENT_DYNCREATE(CMemoryPage, CPropertyPage)

CMemoryPage::CMemoryPage() : CPropertyPage(CMemoryPage::IDD)
{
	//{{AFX_DATA_INIT(CMemoryPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMemoryPage::~CMemoryPage()
{
}

void CMemoryPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMemoryPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMemoryPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMemoryPage)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoryPage message handlers


void CMemoryPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
/*
m_Cursel = 1 CPU
m_Cursel = 2 内存
m_Cursel = 3 GDI
m_Cursel = 4 线程
m_Cursel = 5 GDI分类
m_Cursel = 6 Watch 
m_Cursel = 7 Output 
*/

	g_Gdi.SetDraw(&dc,2);

	if( !g_Gdi.GetFindProcess() )
	{
		GetDlgItem(IDC_NOTICESTATIC)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_NOTICESTATIC)->ShowWindow(SW_HIDE);
	}

}

void CMemoryPage::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	Invalidate();
	CPropertyPage::OnTimer(nIDEvent);
}

int CMemoryPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTimer(2,1000,NULL);

	// TODO: Add your specialized creation code here
	
	return 0;
}
