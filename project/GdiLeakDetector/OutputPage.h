#if !defined(AFX_OUTPUTPAGE_H__E86B2539_2702_403C_8D68_2ADB86B9D5DD__INCLUDED_)
#define AFX_OUTPUTPAGE_H__E86B2539_2702_403C_8D68_2ADB86B9D5DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputPage.h : header file
//
#include "Gdi.h"

extern Gdi g_Gdi;

/////////////////////////////////////////////////////////////////////////////
// COutputPage dialog

class COutputPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COutputPage)

// Construction
public:
	void AddHandleList();
	COutputPage();
	~COutputPage();
	static HWND hWnd;
	static HWND GetOutputPageHandle(){
		return hWnd;
	}
// Dialog Data
	//{{AFX_DATA(COutputPage)
	enum { IDD = IDD_OUTPUT };
	CListCtrl	m_lcLeakHandle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COutputPage)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COutputPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickLeakhandle(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTPAGE_H__E86B2539_2702_403C_8D68_2ADB86B9D5DD__INCLUDED_)
