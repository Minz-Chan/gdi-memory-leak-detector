#if !defined(AFX_CATEGORYSTATISTICSPAGE_H__FFBDDB3E_A454_4557_AD33_8386803EDC8A__INCLUDED_)
#define AFX_CATEGORYSTATISTICSPAGE_H__FFBDDB3E_A454_4557_AD33_8386803EDC8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CategoryStatisticsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCategoryStatisticsPage dialog
#include "Gdi.h"

extern Gdi g_Gdi;

class CCategoryStatisticsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCategoryStatisticsPage)

// Construction
public:
	CCategoryStatisticsPage();
	~CCategoryStatisticsPage();

// Dialog Data
	//{{AFX_DATA(CCategoryStatisticsPage)
	enum { IDD = IDD_CATEGORY_STATISTICS };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCategoryStatisticsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCategoryStatisticsPage)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATEGORYSTATISTICSPAGE_H__FFBDDB3E_A454_4557_AD33_8386803EDC8A__INCLUDED_)
