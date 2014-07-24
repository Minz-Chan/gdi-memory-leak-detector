// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__13CDDF4C_7253_4A4E_9255_10887716C755__INCLUDED_)
#define AFX_MAINFRM_H__13CDDF4C_7253_4A4E_9255_10887716C755__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SplitterWndEx.h"

class CGdiLeakDetectorView;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWndEx m_wndSplitter;
	CSplitterWndEx m_wndSubSplitter;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
	CGdiLeakDetectorView* GetRightPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CDialogBar  m_wndDlgBar;

// Generated message map functions
protected:
	void ShowDebugMsg(LPCTSTR msg);
	BOOL GetProcessDllName(HANDLE hProcess, HMODULE hDll, LPSTR dllName, SIZE_T dllNameSize);
	BOOL m_createdOk;  
	BOOL m_bLoadFile; //打开文件
	BOOL m_bDebug;   //调试

	char m_szCurrPath[256];
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLoadfile();
	afx_msg void OnDebug();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	afx_msg void OnUpdateBtnLoadFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnDebug(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
private:
	CString m_strName;  //文件名
	CString m_strTitle; //标题
	CString m_strPath;  //全路径
	PROCESS_INFORMATION m_procInfo;
	BOOL FindProcess;//找到进程
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__13CDDF4C_7253_4A4E_9255_10887716C755__INCLUDED_)
