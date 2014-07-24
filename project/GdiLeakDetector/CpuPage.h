#if !defined(AFX_CPUPAGE_H__D4F55DA7_544C_4BBA_BA7A_99D777196658__INCLUDED_)
#define AFX_CPUPAGE_H__D4F55DA7_544C_4BBA_BA7A_99D777196658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CpuPage.h : header file
//
#include "Gdi.h"

extern Gdi g_Gdi;
/////////////////////////////////////////////////////////////////////////////
// CCpuPage dialog

class CCpuPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCpuPage)

// Construction
public:
	CCpuPage();
	~CCpuPage();

// Dialog Data
	//{{AFX_DATA(CCpuPage)
	enum { IDD = IDD_CPU };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCpuPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCpuPage)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPUPAGE_H__D4F55DA7_544C_4BBA_BA7A_99D777196658__INCLUDED_)
