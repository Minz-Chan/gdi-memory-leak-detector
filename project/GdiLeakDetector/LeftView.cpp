// LeftView.cpp : implementation file
//

#include "stdafx.h"
#include "gdileakdetector.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CFormView)

CLeftView::CLeftView()
	: CFormView(CLeftView::IDD)
{
	//{{AFX_DATA_INIT(CLeftView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CLeftView::~CLeftView()
{
	m_backHbrush = CreateSolidBrush(RGB(255, 255, 255));
}

void CLeftView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeftView, CFormView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CANCELMODE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

void CLeftView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
//	CRect	rect;   
//	this->GetClientRect(rect);   
//	pDC->FillSolidRect(&rect,RGB(255,255,255));
	
}

HBRUSH CLeftView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	// CTLCOLOR_BTN 3
	// CTLCOLOR_DLG 4
	//	CTLCOLOR_EDIT 1
	//	CTLCOLOR_LISTBOX 2
	//	CTLCOLOR_MSGBOX 0
	//	CTLCOLOR_SCROLLBAR 5

//	if   (nCtlColor==CTLCOLOR_STATIC)       
	{ 
//		pDC-> SetBkMode(TRANSPARENT);
//		pDC-> SetTextColor(RGB(255, 0, 0));
//		return   m_backHbrush; 
    } 
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


void CLeftView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	OnPaint();

	CFormView::OnTimer(nIDEvent);
}

void CLeftView::OnCancelMode() 
{
	CFormView::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetTimer(0, 1000, NULL);
	return 0;
}

void CLeftView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CString str;
	

	SetDlgItemInt( IDC_EDIT_PRO, g_Gdi.GetCpuNumber() );
	SetDlgItemText( IDC_EDIT_VERSION , g_Gdi.GetVersion() );

	str.Format("%ldK",g_Gdi.GetAvalMem() );
	SetDlgItemText( IDC_EDIT_AVALMEM, str );

	str.Format("%ldK",g_Gdi. GetPhyMem ());
	SetDlgItemText( IDC_EDIT_PHYMEM, str);

	str.Format("%ldK", g_Gdi.GetVirtualMem() );
	SetDlgItemText( IDC_EDIT_VIRTUALMEM, str);

	str.Format("%ldK", g_Gdi.GetAvailVirtual() );
	SetDlgItemText( IDC_EDIT_AVALVMEM, str );

	str.Format("%ldK", g_Gdi.GetPageFileSize() );
	SetDlgItemText( IDC_EDIT_PAGEFILESIZE, str );

	str.Format("%ldK", g_Gdi.GetPagePath() );
	SetDlgItemText( IDC_EDIT_PAGEPATH, str);

	// Do not call CFormView::OnPaint() for painting messages
}
