// GdiLeakDetector.h : main header file for the GDILEAKDETECTOR application
//

#if !defined(AFX_GDILEAKDETECTOR_H__E29C7F26_4F17_4971_AC39_7219E945A963__INCLUDED_)
#define AFX_GDILEAKDETECTOR_H__E29C7F26_4F17_4971_AC39_7219E945A963__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGdiLeakDetectorApp:
// See GdiLeakDetector.cpp for the implementation of this class
//

class CGdiLeakDetectorApp : public CWinApp
{
public:
	CGdiLeakDetectorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGdiLeakDetectorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGdiLeakDetectorApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDILEAKDETECTOR_H__E29C7F26_4F17_4971_AC39_7219E945A963__INCLUDED_)
