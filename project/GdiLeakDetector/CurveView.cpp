// CurveView.cpp : implementation file
//

#include "stdafx.h"
#include "gdileakdetector.h"
#include "CurveView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurveView

#define WM_DLLUNLOAD WM_USER + 8

IMPLEMENT_DYNCREATE(CCurveView, CView)

CCurveView::CCurveView():m_CurveSheet("")
{
	
}

CCurveView::~CCurveView()
{
}


BEGIN_MESSAGE_MAP(CCurveView, CView)
	//{{AFX_MSG_MAP(CCurveView)
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurveView drawing

void CCurveView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	
}

/////////////////////////////////////////////////////////////////////////////
// CCurveView diagnostics

#ifdef _DEBUG
void CCurveView::AssertValid() const
{
	CView::AssertValid();
}

void CCurveView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);

	
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCurveView message handlers

void CCurveView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	this->ShowWindow(SW_MAXIMIZE);

	// TODO: Add your specialized code here and/or call the base class
	m_CurveSheet.Create(this, WS_CHILD | WS_VISIBLE, 0);

	if(m_CurveSheet.m_hWnd)
		m_CurveSheet.ShowWindow(SW_MAXIMIZE);

	CRect rect;
	
	GetClientRect(&rect);

	m_CurveSheet.MoveWindow(&rect);

	rect.left -= 3;
	rect.top -= 2;


	CTabCtrl *pTab = m_CurveSheet.GetTabControl();

//	CFont m_fontEdit;

//	m_fontEdit.CreateFont( -8, 0, 0, 0, FW_MEDIUM, 0, 0, 0, DEFAULT_CHARSET, \
//		0, 0, 0, 0, _T("MS Sans Serif") );
//	m_fontEdit.CreateFont(14, 0, 0, 0, 800, TRUE, 0, 0, 1, 0, 0, 0, 0, _T("Arial"));
//	pTab->SetFont( &m_fontEdit );

	pTab->SetWindowPos(NULL, rect.left, rect.top, rect.Width() , rect.Height(),
 			SWP_NOZORDER | SWP_NOACTIVATE);

	//rect.right += 300;
	//rect.bottom += 200;
	
	//m_CurveSheet.SetWindowPos(NULL, rect.left, rect.top, rect.Width() , rect.Height() ,
 	//		SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);

	//::SendMessage(m_CurveSheet.GetTabControl()->GetSafeHwnd(), WM_PAINT, 
	//	(WPARAM)m_CurveSheet.GetTabControl()->GetWindowDC()->GetSafeHdc(), NULL);


	CCurveSheet::hWnd = m_CurveSheet;

}

void CCurveView::OnSize(UINT nType, int cx, int cy) 
{
	//CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
// 	this->ShowWindow(SW_MAXIMIZE);
// 	CWnd *psheet = this->GetWindow(GW_OWNER);//m_messageSheet是resultview的唯一顶层子窗口
// 	if(psheet != NULL){
// 		CRect rect;
// 		this->GetWindowRect(&rect);
// 		psheet->ScreenToClient(&rect);
// 		psheet->SetWindowPos(NULL, 0, 0, rect.Width() , rect.Height() ,
// 			SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
// 		CWnd *ptab = this->m_CurveSheet.GetTabControl();
// 		if(ptab != NULL){
// 			ptab->ScreenToClient(&rect);
// 			ptab->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(),
// 				SWP_NOZORDER | SWP_NOACTIVATE);
// 			
// 			//让PropertySheet自动调整page的大小
// 			this->m_CurveSheet.SetActivePage(this->m_CurveSheet.GetActiveIndex());
// 		}
// 	}
	
}

void CCurveView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	// Do not call CView::OnPaint() for painting messages
}
