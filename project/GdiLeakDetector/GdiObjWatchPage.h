#pragma warning(disable:4786)
#if !defined(AFX_GDIOBJWATCHPAGE_H__D191F2F4_24BD_4A06_8657_B3B9493A9236__INCLUDED_)
#define AFX_GDIOBJWATCHPAGE_H__D191F2F4_24BD_4A06_8657_B3B9493A9236__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GdiObjWatchPage.h : header file
//
#include "Gdi.h"
#include <map>
#include "Struct_Define.h"

using namespace std;

extern Gdi g_Gdi;

/////////////////////////////////////////////////////////////////////////////
// CGdiObjWatchPage dialog

class CGdiObjWatchPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGdiObjWatchPage)
	
	static HWND hWnd;
	static HWND GetGdiObjWatchPageHandle(){
		return hWnd;
	}

// Construction
public:
	void AddObjHandleInfo();
	CGdiObjWatchPage();
	~CGdiObjWatchPage();
	map<string,HANDLE_INFORAMTION> handleMapList;
	// Dialog Data
	//{{AFX_DATA(CGdiObjWatchPage)
	enum { IDD = IDD_GDIOBJWATCH };
	CListCtrl	m_lcHandle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGdiObjWatchPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGdiObjWatchPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickHandle(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDIOBJWATCHPAGE_H__D191F2F4_24BD_4A06_8657_B3B9493A9236__INCLUDED_)
