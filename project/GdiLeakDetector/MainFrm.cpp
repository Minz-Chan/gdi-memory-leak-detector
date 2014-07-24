// MainFrm.cpp : implementation of the CMainFrame class
//
#pragma warning(disable:4786)
#include "stdafx.h"
#include "GdiLeakDetector.h"

#include "MainFrm.h"
#include "GdiLeakDetectorView.h"
#include "LeftView.h"
#include "CurveView.h"
#include "InfoView.h"
#include  "Inject.h"
#include "RightView2.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_SIZE 256




/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDBN_LOADFILE, OnLoadfile)
	ON_BN_CLICKED(IDBN_DEBUG, OnDebug)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
	ON_UPDATE_COMMAND_UI(IDBN_LOADFILE, OnUpdateBtnLoadFile)
	ON_UPDATE_COMMAND_UI(IDBN_DEBUG, OnUpdateBtnDebug)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_createdOk = FALSE;

	GetCurrentDirectory(MAX_SIZE, m_szCurrPath);
	SetCurrExePath(m_szCurrPath);

	FindProcess=FALSE;

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
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

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	m_bLoadFile = TRUE;
	m_bDebug = TRUE;

	SetTimer(0,1000,NULL);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// 创建分割窗
	m_createdOk = m_wndSplitter.CreateStatic(this, 1, 2);
	if(m_createdOk){
		m_createdOk = m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), 
												CSize(0, 0), pContext);
		if(m_createdOk){
			m_createdOk = m_wndSubSplitter.CreateStatic(&m_wndSplitter, 2, 1, 
				WS_CHILD | WS_VISIBLE, m_wndSplitter.IdFromRowCol(0, 1));
			if(m_createdOk){
				m_createdOk = m_wndSubSplitter.CreateView(0, 0, 
					RUNTIME_CLASS(CCurveView), CSize(500, 500), pContext);
				if(m_createdOk){
					m_createdOk = m_wndSubSplitter.CreateView(1, 0, 
						RUNTIME_CLASS(CRightView2), CSize(0, 0), pContext);
					//setLayout();//设置高度
					CRect rect;
					GetClientRect(&rect);
					m_wndSplitter.SetColumnInfo(0, 0.2 * rect.Width(), 0);
					m_wndSplitter.SetColumnInfo(1, 0.8 * rect.Width(), 0);
					m_wndSubSplitter.SetRowInfo(0, 0.5 * rect.Height(), 0);
					m_wndSubSplitter.SetRowInfo(1, 0.5 * rect.Height(), 0);
					m_wndSplitter.RecalcLayout();
					m_wndSubSplitter.RecalcLayout();
				}
			}
		}
	}

	g_Gdi.Updata(0,NULL,false);
	SetWindowText("Gdi Memory Leak Detector");	

	return TRUE;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style   &=   ~WS_THICKFRAME;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style&=~FWS_ADDTOTITLE;
	cs.style &= ~WS_MAXIMIZEBOX;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{

	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

CGdiLeakDetectorView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CGdiLeakDetectorView* pView = DYNAMIC_DOWNCAST(CGdiLeakDetectorView, pWnd);
	return pView;
}

void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	CGdiLeakDetectorView* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}


void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.
	CGdiLeakDetectorView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CGdiLeakDetectorView,
	// process the menu commands...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// other commands change the style on the list control
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
	}
}

void CMainFrame::OnUpdateBtnLoadFile(CCmdUI* pCmdUI){
	if(m_bLoadFile)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateBtnDebug(CCmdUI* pCmdUI){
	if(m_bDebug)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnLoadfile()  //打开文件
{

	// TODO: Add your control notification handler code here
	char BASED_CODE szFilter[] = "EXE File (*.EXE) | *.EXE;*.exe;*.Exe ||";
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | 
		OFN_OVERWRITEPROMPT, szFilter, NULL);

	if (fileDlg.DoModal() == IDCANCEL)
		return;

	::SetWindowText(::GetDlgItem(m_wndDlgBar, IDE_FILEPATH), 
		fileDlg.GetPathName());



}

void CMainFrame::OnDebug() 
{
	// TODO: Add your control notification handler code here
	char szExePath[MAX_SIZE];
	::GetDlgItemText(m_wndDlgBar, IDE_FILEPATH, szExePath, MAX_SIZE);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(si));
	si.cb=sizeof(si);
	

	BOOL bRtn=::CreateProcess(
		NULL,
		szExePath,
		NULL,
		NULL,
		FALSE,
		CREATE_DEFAULT_ERROR_MODE,
		NULL, NULL,
		&si,
		&m_procInfo);
	if(FALSE == bRtn){
		TCHAR chErrMsg[100];
		sprintf(chErrMsg,"Fail to create process !\n Error Code: %ld"
			,GetLastError());
		ShowDebugMsg(_T(chErrMsg));
	}
	

	// 注入GdiDetector.dll，拦截GDI对象的创建和删除调用。
	TCHAR  szFullDllName[MAX_SIZE];
	strcpy(szFullDllName, m_szCurrPath);
	strcat(szFullDllName, "\\GdiDetector.dll");
	
	if(::GetFileAttributes(szFullDllName) != -1){
		// GdiDetector.dll 存在
		//LoadLib(pi.hProcess, szFullDllName);	//注入DLL
		//InjectDll(pi.hProcess, szFullDllName);
		LoadLib(m_procInfo.dwProcessId, szFullDllName);
		FindProcess=true;//找到进程
		g_Gdi.SetFindProcess(FindProcess);
	}
	else{
		ShowDebugMsg(_T("Can not find GdiDetector.dll in current directory."));
		//::CloseHandle(pi.hProcess);
		//::CloseHandle(pi.hThread);
		return;
	}
// 	DEBUG_EVENT dbgEvent;
// 	BOOL bKernel32Loaded=FALSE;
// 	BOOL bGdi32Loaded=FALSE;
// 	BOOL bUser32Loaded=FALSE;
// 	BOOL bThreadStarted=FALSE;
// 
// 	while(1){
// 
// 		DWORD dwContinueStatus = DBG_CONTINUE;
// 		::WaitForDebugEvent(&dbgEvent, INFINITE);
// 		
// 		switch(dbgEvent.dwDebugEventCode){
// 		case EXCEPTION_DEBUG_EVENT:	//调试异常
// 			// 对于非断点异常, dwContinueStatus的值需为DBG_EXCEPTION_NOT_HANDLED
// 			// , 否则被调试进程将被结束
// 			if(dbgEvent.u.Exception.ExceptionRecord.ExceptionCode  
// 				!= EXCEPTION_BREAKPOINT){
// 				dwContinueStatus=DBG_EXCEPTION_NOT_HANDLED;
// 				ShowDebugMsg(_T("Exception occur"));
// 			}
// 			break;
// 
// 		case LOAD_DLL_DEBUG_EVENT:	//载入DLL
// 			TCHAR dllName[64];
// 			GetProcessDllName(pi.hProcess, (HMODULE)dbgEvent.u.LoadDll.lpBaseOfDll, \
// 				dllName, sizeof(dllName));
// 			_strlwr(dllName);
// 			
// 			if(bKernel32Loaded && bGdi32Loaded && bUser32Loaded)
// 			{
// 				if(!bThreadStarted)
// 				{
// 					bThreadStarted = TRUE;
// 
// 					// 注入GdiDetector.dll，拦截GDI对象的创建和删除调用。
// 					TCHAR  szFullDllName[MAX_SIZE];
// 					strcpy(szFullDllName, m_szCurrPath);
// 					strcat(szFullDllName, "\\GdiDetector.dll");
// 
// 					if(::GetFileAttributes(szFullDllName) != -1){
// 							// GdiDetector.dll 存在
// 							//LoadLib(pi.hProcess, szFullDllName);	//注入DLL
// 							InjectDll(pi.hProcess, szFullDllName);
// 					}
// 					else{
// 						ShowDebugMsg(_T("Can not find GdiDetector.dll in current directory."));
// 						::CloseHandle(pi.hProcess);
// 						::CloseHandle(pi.hThread);
// 						return;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				if(strcmp(dllName, "kernel32.dll") == 0)
// 					bKernel32Loaded = TRUE;
// 				else if(strcmp(dllName, "user32.dll") == 0)
// 					bUser32Loaded = TRUE;
// 				else if(strcmp(dllName, "gdi32.dll") == 0)
// 					bGdi32Loaded = TRUE;
// 			}
// 			if(dllName[0])
// 				dllName[0] += 'A' - 'a';//首字母大写
// 			strcat(dllName, " loaded");
// 			//ShowDebugMsg(_T(dllName));
// 			if (dbgEvent.u.LoadDll.hFile)
// 				::CloseHandle(dbgEvent.u.LoadDll.hFile);
// 			break;
// 
// 		case CREATE_PROCESS_DEBUG_EVENT:	//进程创建
// 			ShowDebugMsg(_T("Process Created !"));
// 			if (dbgEvent.u.CreateProcessInfo.hFile)
// 				::CloseHandle(dbgEvent.u.CreateProcessInfo.hFile);
// 			if (dbgEvent.u.CreateProcessInfo.hProcess)
// 				::CloseHandle(dbgEvent.u.CreateProcessInfo.hProcess);
// 			if (dbgEvent.u.CreateProcessInfo.hThread)
// 				::CloseHandle(dbgEvent.u.CreateProcessInfo.hThread);
// 			break;
// 
// 		case EXIT_PROCESS_DEBUG_EVENT:
// 			bThreadStarted = FALSE;
// 			ShowDebugMsg(_T("Process exited !"));
// 			break;
// 		default:
// 			break;
// 		}
// 
// 		::ContinueDebugEvent(dbgEvent.dwProcessId, dbgEvent.dwThreadId, dwContinueStatus);
// 
// 		// 确保被调试进程被杀死
// 		if(EXIT_PROCESS_DEBUG_EVENT == dbgEvent.dwDebugEventCode)
// 			break;
// 	}

	//::CloseHandle(pi.hProcess);
	//::CloseHandle(pi.hThread);
}

void CMainFrame::ShowDebugMsg(LPCTSTR msg)
{
	MessageBox(msg);
}


BOOL CMainFrame::GetProcessDllName(HANDLE hProcess, HMODULE hDll
								   , LPSTR dllName, SIZE_T dllNameSize)
{
	_ASSERTE(dllName != NULL);
	_ASSERTE(dllNameSize >= 24);
	dllName[0] = 0;
	
	SIZE_T bytesRead;
	
	// 获取DOS文件头
	IMAGE_DOS_HEADER dosHdr;
	if (!ReadProcessMemory(hProcess, hDll, &dosHdr, sizeof(dosHdr), &bytesRead))
		return FALSE;
	
	// 获取PE文件头
	IMAGE_NT_HEADERS        ntHdr;
	if (!ReadProcessMemory(hProcess, (PVOID)((char*)hDll+dosHdr.e_lfanew)
		, &ntHdr, sizeof(ntHdr), &bytesRead))
		return FALSE;
	
	// 获取输出表的RVA(相对虚拟地址)
	DWORD exportsRVA = ntHdr.OptionalHeader
		.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]
		.VirtualAddress;
	
	if (!exportsRVA)
		return FALSE;
	
	// 通过RVA取得输出表的数据，置于IMAGE_EXPORT_DIRECTORY结构中
	IMAGE_EXPORT_DIRECTORY  exportDir;
	if (!ReadProcessMemory(hProcess, (PVOID)((char*)hDll+exportsRVA)
		, &exportDir, sizeof(exportDir), &bytesRead))
		return(FALSE);
	
	// 获取DLL名称
	if (!ReadProcessMemory(hProcess, (PVOID)((char*)hDll+exportDir.Name)
		, dllName, dllNameSize, &bytesRead))
		return(FALSE);
	
	return(TRUE);
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if(!g_Gdi.IsProcessWorking() ) //程序是否在运行（未写）
	{
		FindProcess=FALSE;
	}

	if(nIDEvent == 0)
	{
		//是否已经创建进程
		if(FindProcess)
			g_Gdi.Updata(m_procInfo.dwProcessId,m_procInfo.hProcess,true);
		else
			g_Gdi.Updata(0,NULL,false);
		//第一秒过去了（避免CPU第一秒BUG）
		g_Gdi.SetFirstTime(false);

	}
//	this->Invalidate();
	CFrameWnd::OnTimer(nIDEvent);
}
 
