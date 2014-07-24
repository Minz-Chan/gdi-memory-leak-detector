// GdiObjWatchPage.cpp : implementation file
//
#pragma warning(disable:4786)
#include "stdafx.h"
#include "gdileakdetector.h"
#include "GdiObjWatchPage.h"
#include "XMLOperator.h"
#include "RightView2.h"
#include "Inject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGdiObjWatchPage property page
BOOL ReadFromMemory(PHANDLE_INFORMATION lpHandleInfo){
	
	HANDLE g_hMapFile;
	LPVOID g_pBuf;
	
	char szSectionObjectName[] = "HandleOfGdiObj";
	g_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szSectionObjectName);
	//if (g_hMapFile == NULL) 
	//	return FALSE;
	
	g_pBuf = MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);                   
	if (g_pBuf == NULL)
	{
		CloseHandle(g_hMapFile);
		return FALSE;
	}
	
	char *pCur = (char *)g_pBuf;
	
	// 等待状态转换为可读状态
	while(*(int *)pCur){};  
	
	// 状态值
	*(int *)pCur = 1;	//正在读取...
	pCur += sizeof(int);
	
	
	// 句柄值
	lpHandleInfo->handle = *(unsigned long*)pCur;
	pCur += sizeof(unsigned long);
	
	// 类型值
	lpHandleInfo->uType = *(unsigned *)pCur;
	pCur += sizeof(unsigned);
	
	// 参数值
	memcpy(lpHandleInfo->szParaInfo, pCur, 256);
	pCur += (sizeof(char) * 256);
	
	// Registers
	memcpy(&lpHandleInfo->registers, pCur, sizeof(REGISTERS_INFORMATION));
	pCur += sizeof(REGISTERS_INFORMATION);
	
	// Module Information
	int nModule = *(int *)pCur;
	pCur += sizeof(int);
	for(int i=0; i < nModule; i++){
		MODULE_INFORMATION moduleInfo;
		memcpy(&moduleInfo, pCur, sizeof(MODULE_INFORMATION));
		pCur += sizeof(MODULE_INFORMATION);
		lpHandleInfo->moduleInfo.push_back(moduleInfo);
	}
	
	// CallStacks
	int nCallStack = *(int *)pCur;
	pCur += sizeof(int);
	for(int j=0; j < nCallStack; j++){
		char szCallStack[256];
		int nStr = *(int *)pCur;
		pCur += sizeof(int);
		memcpy(szCallStack, pCur, nStr);
		szCallStack[nStr] = 0;
		pCur += nStr;
		lpHandleInfo->callStack.callStack.push_back(szCallStack);
	}
	
	pCur = (char *)g_pBuf;
	
	// 状态值
	*(int *)pCur = 0;	//读取完成
	
	
	return TRUE;
	
}

HWND CGdiObjWatchPage::hWnd = 0;

IMPLEMENT_DYNCREATE(CGdiObjWatchPage, CPropertyPage)

CGdiObjWatchPage::CGdiObjWatchPage() : CPropertyPage(CGdiObjWatchPage::IDD)
{
	//{{AFX_DATA_INIT(CGdiObjWatchPage)
	//}}AFX_DATA_INIT

	
}

CGdiObjWatchPage::~CGdiObjWatchPage()
{
}

void CGdiObjWatchPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGdiObjWatchPage)
	DDX_Control(pDX, IDLC_HANDLE, m_lcHandle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGdiObjWatchPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGdiObjWatchPage)
	ON_NOTIFY(NM_CLICK, IDLC_HANDLE, OnClickHandle)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGdiObjWatchPage message handlers

BOOL CGdiObjWatchPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	
	

	CRect rect;
	this->GetWindowRect(&rect);

	GetDlgItem(IDLC_HANDLE)->MoveWindow(0, 0, rect.right - rect.left
		, rect.bottom - rect.top, FALSE);


	m_lcHandle.SetExtendedStyle(m_lcHandle.GetExtendedStyle() | LVS_EX_FULLROWSELECT 
		| LVS_EX_GRIDLINES | LVS_EX_ONECLICKACTIVATE);

	m_lcHandle.InsertColumn(0, "句柄");
	m_lcHandle.SetColumnWidth(0, rect.right - rect.left - 5);

	
	
	hWnd = this->GetSafeHwnd();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGdiObjWatchPage::OnClickHandle(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	string Handle;
	CString PHandle;
	CRightView2 *View= (CRightView2*)CWnd::FromHandle(CRightView2::hWnd);
	HANDLE_INFORAMTION p;
	map<string,HANDLE_INFORAMTION>::iterator iter;

	int nIndex=m_lcHandle.GetSelectionMark();		//获取当前列表选择的内容，并显示HANDLE_INFORAMTION结构体的内容
	if (nIndex >= 0)
	{
		PHandle=PHandle+m_lcHandle.GetItemText(nIndex,0);
		Handle = (LPCTSTR)PHandle; 
		for(iter = handleMapList.begin(); iter != handleMapList.end(); iter++)
		{
			if (strcmp(iter->first.c_str(),Handle.c_str())==0)		
			{
				View->ShowWatch(iter->second);			
			} 			
		}
		
	}

	*pResult = 0;
}

void CGdiObjWatchPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	g_Gdi.SetCursel(6);	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void CGdiObjWatchPage::AddObjHandleInfo()
{
	
	static int nItem = 0;

	try{
		HANDLE_INFORAMTION handleInfo;
		char szHandle[16];

		ReadFromMemory(&handleInfo);		// 从内存里面读出结构体信息
		
		sprintf(szHandle, "0x%0*X",8,handleInfo.handle);    //转化为16进制

		pair<map<string, HANDLE_INFORAMTION>::iterator,		//判断是否有重复的句柄
			bool> temp = handleMapList.insert(pair<string, 
			HANDLE_INFORAMTION>(szHandle,handleInfo));
		if(temp.second)
			m_lcHandle.InsertItem(nItem++, szHandle);
		
	}
	catch(exception e){
		MessageBox(e.what());
	}
}
