#if !defined(AFX_THREADPAGE_H__A3333374_E319_4AF6_9480_29A852FF8D31__INCLUDED_)
#define AFX_THREADPAGE_H__A3333374_E319_4AF6_9480_29A852FF8D31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThreadPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CThreadPage dialog
#include "Gdi.h"

extern Gdi g_Gdi;
class CThreadPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CThreadPage)

// Construction
public:
	CThreadPage();
	~CThreadPage();

// Dialog Data
	//{{AFX_DATA(CThreadPage)
	enum { IDD = IDD_THREAD };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CThreadPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CThreadPage)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREADPAGE_H__A3333374_E319_4AF6_9480_29A852FF8D31__INCLUDED_)
