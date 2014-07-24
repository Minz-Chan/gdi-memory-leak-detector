// OutputPage.cpp : implementation file
//

#include "stdafx.h"
#include "gdileakdetector.h"
#include "OutputPage.h"
#include "XMLOperator.h"
#include "RightView2.h"
#include "Inject.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputPage property page

HWND COutputPage::hWnd = 0;

IMPLEMENT_DYNCREATE(COutputPage, CPropertyPage)

COutputPage::COutputPage() : CPropertyPage(COutputPage::IDD)
{
	//{{AFX_DATA_INIT(COutputPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

COutputPage::~COutputPage()
{
}

void COutputPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputPage)
	DDX_Control(pDX, IDLC_LEAKHANDLE, m_lcLeakHandle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutputPage, CPropertyPage)
	//{{AFX_MSG_MAP(COutputPage)
	ON_NOTIFY(NM_CLICK, IDLC_LEAKHANDLE, OnClickLeakhandle)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputPage message handlers

BOOL COutputPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	CRect rect;
	this->GetWindowRect(&rect);
	
	GetDlgItem(IDLC_LEAKHANDLE)->MoveWindow(0, 0, rect.right - rect.left
		, rect.bottom - rect.top, FALSE);
	
	
	m_lcLeakHandle.SetExtendedStyle(m_lcLeakHandle.GetExtendedStyle() | LVS_EX_FULLROWSELECT 
		| LVS_EX_GRIDLINES | LVS_EX_ONECLICKACTIVATE);


	m_lcLeakHandle.InsertColumn(0, "泄漏的句柄");
	m_lcLeakHandle.SetColumnWidth(0, rect.right - rect.left - 5);
	
	hWnd = this->GetSafeHwnd();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COutputPage::OnClickLeakhandle(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
//	CRightView2.m_OutputList.InsertItem(1,"bb");
	CRightView2 *View= (CRightView2*)CWnd::FromHandle(CRightView2::hWnd);
/*	a->m_OutputList.InsertItem(1,"bb");
	a->m_OutputList.SetColumnWidth(0,80); */


	xmloperator xml;
	string Handle;
	CString CHandle;
	char szPath[256];

	sprintf(szPath, "%s\\InfoOfLeakObject.xml", GetCurrExePath());		//获取当前列表选择的内容，并显示HANDLE_INFORAMTION结构体的内容
	xml.Start(szPath);	
	int nIndex=m_lcLeakHandle.GetSelectionMark();
	if (nIndex >= 0)
	{
		CHandle = CHandle + m_lcLeakHandle.GetItemText(nIndex, 0);
		Handle = (LPCTSTR)CHandle; 
		View->ShowData(Handle);
	}
	xml.End(szPath);

	*pResult = 0;
}

void COutputPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	g_Gdi.SetCursel(7);	
	// Do not call CPropertyPage::OnPaint() for painting messages
}


void COutputPage::AddHandleList()
{


	m_lcLeakHandle.DeleteAllItems();		//清空列表上的内容

	xmloperator xml;
	string temp,AllHandle;
	int i;
	char szPath[256];
	vector<string> StrHandle;
	vector<string>::iterator  iterHandle;

	sprintf(szPath, "%s\\InfoOfLeakObject.xml", GetCurrExePath());		//从InfoOfLeakObject.xml文件中读出所有的句柄
	xml.Start(szPath);	
	xml.Find_All_Data(temp, AllHandle);	
	xml.End(szPath);

	xml.Start(szPath);	
	StrHandle=xml.Split(AllHandle, "@", true, false);		//将AllHandle以“@”分割，并插入到列表里面
	for (i=0, iterHandle = StrHandle.begin(); iterHandle != StrHandle.end(); iterHandle++, i++)
	{
		m_lcLeakHandle.InsertItem(i, (*iterHandle).c_str());			
	}
	xml.End(szPath);
}

BOOL COutputPage::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class

	

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
