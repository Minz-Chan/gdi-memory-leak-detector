#if !defined(AFX_GDINUMBERPAGE_H__1AB657CC_6950_4018_A688_7339173EBBA1__INCLUDED_)
#define AFX_GDINUMBERPAGE_H__1AB657CC_6950_4018_A688_7339173EBBA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GdiNumberPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGdiNumberPage dialog 
#include "Gdi.h"

extern Gdi g_Gdi;

class CGdiNumberPage : public CPropertyPage
{ 
	DECLARE_DYNCREATE(CGdiNumberPage)

// Construction
public:
	CGdiNumberPage();
	~CGdiNumberPage();

// Dialog Data
	//{{AFX_DATA(CGdiNumberPage)
	enum { IDD = IDD_GDINUMBER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGdiNumberPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGdiNumberPage)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDINUMBERPAGE_H__1AB657CC_6950_4018_A688_7339173EBBA1__INCLUDED_)
