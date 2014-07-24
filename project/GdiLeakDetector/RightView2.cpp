// RightView2.cpp : implementation file
//



#include "stdafx.h"
#include <fstream.h>
#include "gdileakdetector.h"
#include "RightView2.h"
#include "OutputPage.h"
#include "GdiObjWatchPage.h"
#include "Inject.h"
#include "CurveSheet.h"
#include "CodeShow.h"
#include "XMLOperator.h"
#include "Struct_Define.h"
#include  <map>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRightView2 

#define WM_DLLUNLOAD WM_USER + 8
#define WM_GDIOBJACTION WM_USER + 9

IMPLEMENT_DYNCREATE(CRightView2, CFormView)


HWND CRightView2::hWnd = 0;


CRightView2::CRightView2()
	: CFormView(CRightView2::IDD)
{
	//{{AFX_DATA_INIT(CRightView2)
	//}}AFX_DATA_INIT

}

CRightView2::~CRightView2()
{
	if(g_pBuf){
		UnmapViewOfFile(g_pBuf);
		CloseHandle(g_hMapFile);
	}
}

void CRightView2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRightView2)
	DDX_Control(pDX, IDC_MEMORYEDIT, m_MemoryEdit);
	DDX_Control(pDX, IDC_WATCHEDIT, m_WatchEdit);
	DDX_Control(pDX, IDC_OUTPUTLIST, m_OutputList);
	DDX_Control(pDX, IDC_LIST2, m_GdiTpyeList);
	DDX_Control(pDX, IDC_LIST1, m_GdiList);
	DDX_Control(pDX, IDC_LIST3, m_ThreadList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRightView2, CFormView)
	//{{AFX_MSG_MAP(CRightView2)
	ON_WM_PAINT()
	ON_WM_CANCELMODE()
	ON_WM_CREATE()
	ON_WM_CAPTURECHANGED()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_OUTPUTLIST, OnDblclkOutputlist)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DLLUNLOAD, OnDLLUnLoaded)
	ON_MESSAGE(WM_GDIOBJACTION, OnGdiObjAction)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRightView2 diagnostics

#ifdef _DEBUG
void CRightView2::AssertValid() const
{
	CFormView::AssertValid();
}

void CRightView2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRightView2 message handlers

void CRightView2::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	

	char szSectionObjectName[] = "HandleOfMain";
	
	g_hMapFile = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // 
		512,                // buffer size  
		szSectionObjectName);  // name of mapping object
	
	TCHAR message[256];
	if (g_hMapFile == NULL)
	{ 
		::wsprintf(message, _T("Debugger could not open file mapping object (error = %d)."),
			GetLastError());
		AfxMessageBox(message);
		PostQuitMessage(1);
	}
	
	g_pBuf = (LPTSTR)::MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS,0,0,128);           
	if (g_pBuf == NULL)
	{
		CloseHandle(g_hMapFile);
		::wsprintf(message, _T("Debugger could not map view of file (error = %d)."),
			GetLastError());
		AfxMessageBox(message);
		PostQuitMessage(1);
	}
	
	*(DWORD*)g_pBuf = (DWORD)m_hWnd;

	char szCurrPath[256];
	char *pCur = (char *)g_pBuf;
	pCur += sizeof(DWORD);
	::GetCurrentDirectory(256, szCurrPath);
	::memset(pCur, 0, 256);
	::memcpy(pCur, szCurrPath, 256);


	//m_GdiList为一个和CListCtrl绑定的变量
	//在控件属性中指定控件为报表风格
	//首先设定列表控件风格可以整行选中，并具有作业线

	////////////////////////////////////////////////////////////////////////////////////////
	//GDI
	DWORD style=m_GdiList.GetExtendedStyle();
	m_GdiList.SetExtendedStyle(style|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_GdiList.InsertColumn(1,"编号");
	m_GdiList.SetColumnWidth(0,80); 

	m_GdiList.InsertColumn(2,"句柄"); 
	m_GdiList.SetColumnWidth(1,120); 

	m_GdiList.InsertColumn(3,"对象类型");
	m_GdiList.SetColumnWidth(2,120); 

	m_GdiList.InsertColumn(4,"内核地址");
	m_GdiList.SetColumnWidth(3,120); 

	m_GdiList.InsertColumn(5,"扩展信息");
	m_GdiList.SetColumnWidth(4,160); 

	///////////////////////////////////////////////////////////////////////////////////////////
	// GDI类型
	DWORD style2=m_GdiTpyeList.GetExtendedStyle();
	m_GdiTpyeList.SetExtendedStyle(style2|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//以下为插入几个列的列标题
	m_GdiTpyeList.InsertColumn(1,"对象类型");
	m_GdiTpyeList.SetColumnWidth(0,80); 

	m_GdiTpyeList.InsertColumn(2,"对象数量");
	m_GdiTpyeList.SetColumnWidth(1,120); 

	m_GdiTpyeList.InsertColumn(3,"扩展信息");
	m_GdiTpyeList.SetColumnWidth(2,160); 

	///////////////////////////////////////////////////////////////////////////////////////////
	// 线程
	DWORD style3=m_ThreadList.GetExtendedStyle();
	m_ThreadList.SetExtendedStyle(style2|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_ThreadList.InsertColumn(1,"编号");
	m_ThreadList.SetColumnWidth(0,80); 

	m_ThreadList.InsertColumn(2,"线程ID");
	m_ThreadList.SetColumnWidth(1,120); 

	m_ThreadList.InsertColumn(3,"优先级");
	m_ThreadList.SetColumnWidth(2,120); 

	/////////////////////////////////////////////////////////////////////////////////////////////
	//  output
	DWORD style4=m_OutputList.GetExtendedStyle();
	m_OutputList.SetExtendedStyle(style2|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	//string s="111";
	//m_OutputList.InsertItem(0,s.c_str());

	CRect rect;
	this->GetOwner()->GetWindowRect(&rect);

	
	m_OutputList.InsertColumn(0, "Handle");
	m_OutputList.SetColumnWidth(0, rect.right - rect.left - 10); 
	
	/*m_OutputList.InsertColumn(2,"t");
	m_OutputList.SetColumnWidth(1,120); 

	m_OutputList.InsertColumn(3,"p");
	m_OutputList.SetColumnWidth(2,120); */
	////////////////////////////////////////////////////////////////////////////////////////////
	//

	CRightView2::hWnd = m_hWnd;

	IsWatchPageInit = FALSE;
}


/*
m_Cursel = 1 CPU
m_Cursel = 2 内存
m_Cursel = 3 GDI
m_Cursel = 4 线程
m_Cursel = 5 GDI分类
m_Cursel = 6 Watch 
m_Cursel = 7 Output 
*/
void CRightView2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	m_Cursel = g_Gdi.GetCursel();
	UpdataList(); //更新控件
	ListMove(); //调整控件位置
	




	// Do not call CFormView::OnPaint() for painting messages
}

void CRightView2::OnCancelMode() 
{
	CFormView::OnCancelMode();
}


int CRightView2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	SetTimer(0,5000,NULL); //5秒刷一次信息
	SetTimer(1,100,NULL);  //时刻判断属性页是否更换
	SetTimer(2,1000,NULL); //1秒刷一次信息
	return 0;
}

void CRightView2::OnCaptureChanged(CWnd *pWnd) 
{

	CFormView::OnCaptureChanged(pWnd);
}

void CRightView2::OnTimer(UINT nIDEvent) 
{

	if(nIDEvent == 1 && m_Cursel != g_Gdi.GetCursel() )
	{
		m_Cursel = g_Gdi.GetCursel();
		this->Invalidate();
		
	}
	
	if( nIDEvent == 0 &&( g_Gdi.GetCursel() == 5 || g_Gdi.GetCursel() == 4 || g_Gdi.GetCursel() == 3 ))
	{
		this->Invalidate();
	}
	if(nIDEvent == 2 &&  g_Gdi.GetCursel() == 2)
	{
		this->Invalidate(true);
	}
	CFormView::OnTimer(nIDEvent);
}






/*
m_Cursel = 1 CPU
m_Cursel = 2 内存
m_Cursel = 3 GDI
m_Cursel = 4 线程
m_Cursel = 5 GDI分类
m_Cursel = 6 Watch 
m_Cursel = 7 Output 
*/

void CRightView2::UpdataList() //更新列表控件
{


	ShowControl(); //显示或隐藏相关控件

	if( m_Cursel == 2)
	{
		CString str;
		str.Format("\r\n\r\n\r\n\r\t\r\t\r\t此刻本进程内存使用量：%ld KB ",g_Gdi.GetProcessMem() );
		m_MemoryEdit.SetWindowText(str);
	}
	if( m_Cursel == 3)  //GDI
	{

		int j = 0 ;
		m_GdiList.SetRedraw(FALSE);
		vGDIinfo = g_Gdi.GetGDIInfo();
		vector<GDIINFO>::iterator vi = vGDIinfo.begin() ;

		m_GdiList.DeleteAllItems();
	
		for( ;  vi != vGDIinfo.end() ; vi++, j++)
		{
			m_GdiList.InsertItem(j,"");//首先插入一行
			//更新
			CString str;
			str.Format("%d",j+1);
			m_GdiList.SetItemText(j,0,str);//设置第一列数据

			str.Format("0x%0x",vi->handle);
			m_GdiList.SetItemText(j,1,str);//设置第二列数据
			
			m_GdiList.SetItemText(j,2,vi->type);//设置第三列数据

			str.Format("0x%0x",vi->pKernel);
			m_GdiList.SetItemText(j,3,str);//设置第四列数据
		}
		m_GdiList.SetRedraw(TRUE);
		m_GdiList.SetFocus();
		m_GdiList.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}
	else if( m_Cursel == 5) // GDI类型
	{

		m_GdiTpyeList.SetRedraw(FALSE);

		vector<GDITYPEINFO> vGDITypeinfo = g_Gdi.GetGDITypeInfo();
		int j=0;
		m_GdiTpyeList.DeleteAllItems();

		while(!vGDITypeinfo.empty())
		{
			m_GdiTpyeList.InsertItem(j,"");//首先插入一行
			//更新
			CString str;
			
			m_GdiTpyeList.SetItemText(j, 0, vGDITypeinfo.back().type);//设置第一列数据
			
			str.Format("%ld", vGDITypeinfo.back().num);
			m_GdiTpyeList.SetItemText(j, 1, str);//设置第二列数据
			
			str = vGDITypeinfo.back().Expand;
			m_GdiTpyeList.SetItemText(j, 2, str);//设置第三列数据
			vGDITypeinfo.pop_back();
			j++;
		}

		m_GdiTpyeList.SetRedraw(TRUE);
	}
	else if( m_Cursel == 4)  //线程
	{
		int j = 0 ;

		vThreadinfo = g_Gdi.GetThreadInfo();

		m_ThreadList.SetRedraw(FALSE);
		m_ThreadList.DeleteAllItems();

	
		while(!vThreadinfo.empty())
		{
			m_ThreadList.InsertItem(j,"");//首先插入一行
			//更新
			CString str;
			str.Format("%d", j+1);
			m_ThreadList.SetItemText(j, 0, str );//设置第一列数据
			str.Format("%ld", vThreadinfo.back().id);
			m_ThreadList.SetItemText(j, 1, str );//设置第一列数据
			
			str.Format("%ld", vThreadinfo.back().pri);
			m_ThreadList.SetItemText(j, 2, str );//设置第二列数据
			
			vThreadinfo.pop_back();
			j++;
		}
		m_ThreadList.SetRedraw(TRUE);
	}
	else if(m_Cursel == 6)
	{
		//output
	}
	else if(m_Cursel == 7)
	{
		//watch
	}
}


void CRightView2::ListMove() //调整对话框
{
	CRect dlgRect;	
	this->GetWindowRect(&dlgRect); 
	
	GetDlgItem(IDC_LIST1)->MoveWindow(0, 0 ,  //GDI
		dlgRect.right - dlgRect.left , dlgRect.bottom - dlgRect.top,FALSE);
	GetDlgItem(IDC_LIST3)->MoveWindow(0 , 0 , //线程
		dlgRect.right - dlgRect.left  , dlgRect.bottom - dlgRect.top,FALSE);
	GetDlgItem(IDC_LIST2)->MoveWindow(0 , 0 , //GDI类型
		dlgRect.right - dlgRect.left  , dlgRect.bottom - dlgRect.top,FALSE);
	GetDlgItem(IDC_WATCHEDIT)->MoveWindow(0 , 0 ,  //WATCH
		dlgRect.right - dlgRect.left  , dlgRect.bottom - dlgRect.top,FALSE);
	GetDlgItem(IDC_OUTPUTLIST)->MoveWindow(0 , 0 ,  //OUTPUT
		dlgRect.right - dlgRect.left  , dlgRect.bottom - dlgRect.top,FALSE);
	GetDlgItem(IDC_MEMORYEDIT)->MoveWindow(0 , 0 ,  //OUTPUT
		dlgRect.right - dlgRect.left  , dlgRect.bottom - dlgRect.top,FALSE);
}

/*
m_Cursel = 1 CPU
m_Cursel = 2 内存
m_Cursel = 3 GDI
m_Cursel = 4 线程
m_Cursel = 5 GDI分类
m_Cursel = 6 Watch 
m_Cursel = 7 Output 
*/

void CRightView2::ShowControl()
{
	map<int,CWnd *> mapControl;

	// 关联map
	mapControl.insert( make_pair(2, &m_MemoryEdit));
	mapControl.insert( make_pair(3, &m_GdiList));
	mapControl.insert( make_pair(4, &m_ThreadList));
	mapControl.insert( make_pair(5, &m_GdiTpyeList));
	mapControl.insert( make_pair(6, &m_WatchEdit));
	mapControl.insert( make_pair(7, &m_OutputList));

	//提示信息隐藏


	//隐藏显示相关控件
	for(int i=2; i<=7; i++)
	{
		if(i != m_Cursel )
		{
			mapControl[i]->ShowWindow(SW_HIDE);
		}

		else
		{
			if( g_Gdi.GetFindProcess() )
			{

				GetDlgItem(IDC_NOTICESTATIC)->ShowWindow(SW_HIDE); //若已经打开进程 隐藏
				mapControl[i]->ShowWindow(SW_SHOW);
			}
			else
			{
				
				GetDlgItem(IDC_NOTICESTATIC)->ShowWindow(SW_SHOW); //若还没打开进程 显示
			}
		}
	}

	if(m_Cursel == 1  ) //如果是CPU和内存属性页
	{
		GetDlgItem(IDC_NOTICESTATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_NOTICESTATIC2)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_NOTICESTATIC2)->ShowWindow(SW_HIDE);
	}
}



void CRightView2::ShowData(string Handle)
{
	m_OutputList.DeleteAllItems();

	xmloperator xml;	
	string temp;
	int nItem;
	HANDLE_INFORAMTION p;
	char szPath[256];
	vector<string> strContent,strTab;
	vector<string>::iterator  iterContent,iterTab;

	sprintf(szPath, "%s\\InfoOfLeakObject.xml", GetCurrExePath());		//从InfoOfLeakObject.xml文件中读出，句柄为Handle的HANDLE_INFORAMTION结构体
	xml.Start(szPath);
	xml.ShowHandle(Handle, temp);	
	iterContent = xml.Stack.begin();	
	xml.test = (*iterContent);		
	xml.End(szPath);
	xml.Start(szPath);
	xml.Read(&p);

	strContent=xml.Split(xml.Show_Data(&p), "\r\n", true, false);		//将HANDLE_INFORAMTION结构体信息显示出来
	for(nItem=0, iterContent = strContent.begin(); iterContent != strContent.end(); iterContent++, nItem++)
	{
		strTab = xml.Split((*iterContent), "@", true, false);
		for(iterTab = strTab.begin(); iterTab != strTab.end(); iterTab++)
		{
		if(strcmp((*iterTab).c_str(), "!") != 0)
		m_OutputList.InsertItem(nItem, (*iterTab).c_str());
		iterTab++;
		if(iterTab!=strTab.end())
		{
		(*iterTab)="         "+(*iterTab);
		m_OutputList.InsertItem(nItem, (*iterTab).c_str());
		}
		else iterTab--;
		}
	}
	xml.End(szPath);

}

LRESULT CRightView2::OnDLLUnLoaded(WPARAM wParam, LPARAM lParam){

	CCurveSheet *pCurveSheet = (CCurveSheet *)CWnd::FromHandle(CCurveSheet::hWnd);
	pCurveSheet->SetActivePage(6);

	COutputPage *pOutputPage = (COutputPage *)CWnd::FromHandle(COutputPage::hWnd);
	pOutputPage->AddHandleList();

	return 0;
}


LRESULT CRightView2::OnGdiObjAction(WPARAM wParam, LPARAM lParam){

	//::MessageBox(NULL, "Gdi Object action", "Message", MB_ICONINFORMATION);

	CCurveSheet *pCurveSheet = (CCurveSheet *)CWnd::FromHandle(CCurveSheet::hWnd);
	int nActivePage = pCurveSheet->GetActiveIndex();

	// 未初始化时, 激活, 让其初始化
	if(!IsWatchPageInit)	
		pCurveSheet->SetActivePage(5);

	pCurveSheet->SetActivePage(nActivePage);

	CGdiObjWatchPage * pGdiObjWatchPage = (CGdiObjWatchPage *)CWnd::FromHandle(
		CGdiObjWatchPage::hWnd);

	pGdiObjWatchPage->AddObjHandleInfo();

	return 0;
}

void CRightView2::ShowWatch(HANDLE_INFORAMTION p)
{
	xmloperator xml;
	vector<string>::iterator  iterContent;
	vector<string> strContent;
	string Content;

	strContent = xml.Split(xml.Show_Data(&p), "!@", true, false);		//显示HANDLE_INFORAMTION结构体的信息
	for(iterContent=strContent.begin();iterContent!=strContent.end();iterContent++)
	{
		Content = Content + (*iterContent) + "\r\t";
	}	
	m_WatchEdit.SetWindowText(Content.c_str());

}






void CRightView2::OnDblclkOutputlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	

	int nIndex=m_OutputList.GetSelectionMark();
	CString szLineText = m_OutputList.GetItemText(nIndex, 0);


	if(!(szLineText.Find(":") != -1 && szLineText.Find("\\") != -1
		&& szLineText.Find("(") != -1))
		return;


	int nLine = atoi(szLineText.Mid(szLineText.FindOneOf("("), 
		szLineText.FindOneOf(")") - szLineText.FindOneOf("(")).GetBuffer(0) + 1);
	
	szLineText.TrimLeft(" ");
	szLineText = szLineText.Mid(0, szLineText.FindOneOf("(") - 1);

	CCodeShow dlgCodeShow;

	AfxGetMainWnd()->ShowWindow(SW_SHOWMINIMIZED);
	::SetWindowPos(dlgCodeShow.GetSafeHwnd(), HWND_TOPMOST, 
		0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);

	dlgCodeShow.SetFileName(szLineText);
	dlgCodeShow.SetLineNum(nLine);

	if(dlgCodeShow.DoModal() == IDCANCEL)
		AfxGetMainWnd()->ShowWindow(SW_SHOWNORMAL);


	*pResult = 0;
}
