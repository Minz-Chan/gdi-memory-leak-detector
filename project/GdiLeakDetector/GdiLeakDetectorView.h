// GdiLeakDetectorView.h : interface of the CGdiLeakDetectorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDILEAKDETECTORVIEW_H__5DC0CDCF_293A_4216_AFA9_B5AC54813B23__INCLUDED_)
#define AFX_GDILEAKDETECTORVIEW_H__5DC0CDCF_293A_4216_AFA9_B5AC54813B23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SplitterWndEx.h"
#include "LeftView.h"
#include "GdiLeakDetectorDoc.h"

class CGdiLeakDetectorView : public CListView
{
protected: // create from serialization only
	CGdiLeakDetectorView();
	DECLARE_DYNCREATE(CGdiLeakDetectorView)

// Attributes
public:
	CGdiLeakDetectorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGdiLeakDetectorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGdiLeakDetectorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	CDialogBar m_wndDlgBar;
	CReBar m_wndReBar;
	void SwitchView();
	CSplitterWndEx m_wndSubSplitter;
	//{{AFX_MSG(CGdiLeakDetectorView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GdiLeakDetectorView.cpp
inline CGdiLeakDetectorDoc* CGdiLeakDetectorView::GetDocument()
   { return (CGdiLeakDetectorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDILEAKDETECTORVIEW_H__5DC0CDCF_293A_4216_AFA9_B5AC54813B23__INCLUDED_)
