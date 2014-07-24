#if !defined(AFX_MEMORYPAGE_H__653D43A8_FD8F_4AB5_89A5_5A3A03584559__INCLUDED_)
#define AFX_MEMORYPAGE_H__653D43A8_FD8F_4AB5_89A5_5A3A03584559__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MemoryPage.h : header file
//
#include "Gdi.h"

extern Gdi g_Gdi;
/////////////////////////////////////////////////////////////////////////////
// CMemoryPage dialog

class CMemoryPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMemoryPage)

// Construction
public:
	CMemoryPage();
	~CMemoryPage();

// Dialog Data
	//{{AFX_DATA(CMemoryPage)
	enum { IDD = IDD_MEMORY };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMemoryPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMemoryPage)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMORYPAGE_H__653D43A8_FD8F_4AB5_89A5_5A3A03584559__INCLUDED_)
