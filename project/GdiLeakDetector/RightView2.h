#if !defined(AFX_RIGHTVIEW2_H__715C302B_EDDF_4D00_ADBD_CE0ECC1758F3__INCLUDED_)
#define AFX_RIGHTVIEW2_H__715C302B_EDDF_4D00_ADBD_CE0ECC1758F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RightView2.h : header file
//
#pragma   warning   (disable:4786)
/////////////////////////////////////////////////////////////////////////////
// CRightView2 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "Gdi.h"
#include "Struct_Define.h"
extern Gdi g_Gdi; 

class CRightView2 : public CFormView
{
protected:
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRightView2)
	CRightView2(); 
	static HWND hWnd;
	static HWND GetRightViewHandle(){
		return hWnd;
	}
// Form Data
public:
	//{{AFX_DATA(CRightView2)
	enum { IDD = IDD_FORMVIEW1 };
	CEdit	m_MemoryEdit;
	CEdit	m_WatchEdit;
	CListCtrl	m_OutputList;
	CListCtrl	m_GdiTpyeList;
	CListCtrl	m_GdiList;
	CListCtrl	m_ThreadList;
	//}}AFX_DATA
  
// Attributes
public:

// Operations
public:
	void ShowWatch(HANDLE_INFORAMTION p);
	void ShowData(string Handle);
	void ShowControl();
	void ListMove();
	void UpdataList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRightView2)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRightView2(); 
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CRightView2)
	afx_msg void OnPaint();
	afx_msg void OnCancelMode();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDblclkOutputlist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnDLLUnLoaded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGdiObjAction(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	DWORD m_Cursel;
	vector<GDIINFO> vGDIinfo;
	vector<THREADINFO> vThreadinfo;
	int num;
	DWORD m_GdiNum;
	HANDLE g_hMapFile;
	LPVOID g_pBuf;
	BOOL IsWatchPageInit;
};

/*
m_Cursel = 1 CPU
m_Cursel = 2 内存
m_Cursel = 3 GDI
m_Cursel = 4 线程
m_Cursel = 5 GDI分类
m_Cursel = 6 Watch 
m_Cursel = 7 Output 
*/

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIGHTVIEW2_H__715C302B_EDDF_4D00_ADBD_CE0ECC1758F3__INCLUDED_)
