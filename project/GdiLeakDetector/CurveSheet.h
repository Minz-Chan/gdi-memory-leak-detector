#if !defined(AFX_CURVESHEET_H__83B74520_DC77_455B_A799_F9C57A9A45A8__INCLUDED_)
#define AFX_CURVESHEET_H__83B74520_DC77_455B_A799_F9C57A9A45A8__INCLUDED_

#include "CategoryStatisticsPage.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurveSheet.h : header file
//

#include "CategoryStatisticsPage.h"
#include "CpuPage.h"
#include "MemoryPage.h"
#include "GdiNumberPage.h"
#include "ThreadPage.h"
#include "GdiObjWatchPage.h"
#include "OutputPage.h"

/////////////////////////////////////////////////////////////////////////////
// CCurveSheet

class CCurveSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCurveSheet)
	static HWND hWnd;
	static HWND GetCurveSheetHandle(){
		return hWnd;
	}
// Construction
public:
	CCurveSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CCurveSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurveSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCurveSheet();

	// Generated message map functions
protected:
	CCategoryStatisticsPage m_CategoryStatistics;
	CCpuPage m_Cpu;
	CMemoryPage m_Memory;
	CGdiNumberPage m_GdiNumber;
	CThreadPage m_Thread;
	CGdiObjWatchPage m_ObjWatch;
	COutputPage m_Output;

	//{{AFX_MSG(CCurveSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURVESHEET_H__83B74520_DC77_455B_A799_F9C57A9A45A8__INCLUDED_)
