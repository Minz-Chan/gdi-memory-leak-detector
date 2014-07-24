// GdiLeakDetectorView.cpp : implementation of the CGdiLeakDetectorView class
//

#include "stdafx.h"
#include "GdiLeakDetector.h"

#include "GdiLeakDetectorDoc.h"
#include "GdiLeakDetectorView.h"
#include "CurveView.h"
#include "InfoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGdiLeakDetectorView

IMPLEMENT_DYNCREATE(CGdiLeakDetectorView, CListView)

BEGIN_MESSAGE_MAP(CGdiLeakDetectorView, CListView)
	//{{AFX_MSG_MAP(CGdiLeakDetectorView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGdiLeakDetectorView construction/destruction

CGdiLeakDetectorView::CGdiLeakDetectorView()
{
	// TODO: add construction code here
	
}

CGdiLeakDetectorView::~CGdiLeakDetectorView()
{
}

BOOL CGdiLeakDetectorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style   &=   ~WS_THICKFRAME;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGdiLeakDetectorView drawing

void CGdiLeakDetectorView::OnDraw(CDC* pDC)
{
	CGdiLeakDetectorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CListCtrl& refCtrl = GetListCtrl();
	refCtrl.InsertItem(0, "Item!");
	// TODO: add draw code for native data here
	
	//GetDlgItem(IDBN_LOADFILE)->EnableWindow(TRUE);
}

void CGdiLeakDetectorView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
	//GetDlgItem(IDBN_LOADFILE)->EnableWindow(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CGdiLeakDetectorView diagnostics

#ifdef _DEBUG
void CGdiLeakDetectorView::AssertValid() const
{
	CListView::AssertValid();
}

void CGdiLeakDetectorView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CGdiLeakDetectorDoc* CGdiLeakDetectorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGdiLeakDetectorDoc)));
	return (CGdiLeakDetectorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGdiLeakDetectorView message handlers
void CGdiLeakDetectorView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}



void CGdiLeakDetectorView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	
	SwitchView();

	// TODO: Add your message handler code here
	
}

void CGdiLeakDetectorView::SwitchView()
{
	CRect rect;
	
	GetClientRect(&rect);
	
    int cx = rect.Width();
	int cy = rect.Height();
	
	m_wndSubSplitter.MoveWindow(-2, -2, cx, cy+3);
	m_wndSubSplitter.SetRowInfo(0, 0.6 * cy, 0);
	m_wndSubSplitter.SetRowInfo(1, 0.4*cy , 0);
	m_wndSubSplitter.RecalcLayout();
}

int CGdiLeakDetectorView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	CCreateContext *pContext = (CCreateContext *)lpCreateStruct->lpCreateParams;
	
	if(!m_wndSubSplitter.CreateStatic(this, 2, 1)){
		TRACE0("Fail to create sub spliter !");
		return FALSE;
	}
	
	
	if (/*!m_wndSubSplitter.CreateView(0, 0, RUNTIME_CLASS(CTabView), CSize(10, 10), pContext) ||*/
		!m_wndSubSplitter.CreateView(0, 0, RUNTIME_CLASS(CCurveView), CSize(200, 200), pContext) ||
		!m_wndSubSplitter.CreateView(1, 0, RUNTIME_CLASS(CInfoView), CSize(300, 300), pContext))
	{
		m_wndSubSplitter.DestroyWindow();
		return FALSE;
	}

	//m_wndSubSplitter.SetActivePane(0,0);
	//m_wndSubSplitter.SetActivePane(1,0);

	if (!m_wndDlgBar.Create(this, IDT_LOADVIEW, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}
	
	if (!m_wndReBar.Create(this) ||
		/*!m_wndReBar.AddBar(&m_wndToolBar) ||*/
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
	
	m_wndReBar.SetActiveWindow();
	
	return 0;
}

HBRUSH CGdiLeakDetectorView::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	if   (nCtlColor==CTLCOLOR_STATIC)       
	{ 
		// 		pDC-> SetBkMode(TRANSPARENT); 
		// 		return   m_backHbrush; 
		pDC->SetTextColor(RGB(255,0,0));   
		CFont   font;   
		LOGFONT   lf;   
		memset(&lf,   0,   sizeof(LOGFONT));               //   清零   
		lf.lfHeight   =   24;                                             //   设置字体高度   
		strcpy(lf.lfFaceName,   "Arial");                 //   设置字形   
		font.CreateFontIndirect(&lf);     //   生成字体   
		pDC->SelectObject(&font);   
    } 
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

