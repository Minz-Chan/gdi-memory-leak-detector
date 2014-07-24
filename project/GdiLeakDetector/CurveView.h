#if !defined(AFX_CURVEVIEW_H__CECC7332_2E4B_4146_93F0_2A660B3DC5AA__INCLUDED_)
#define AFX_CURVEVIEW_H__CECC7332_2E4B_4146_93F0_2A660B3DC5AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurveView.h : header file
//

#include "CurveSheet.h"//ÇúÏß±í

/////////////////////////////////////////////////////////////////////////////
// CCurveView view

class CCurveView : public CView
{
protected:
	CCurveView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCurveView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurveView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCurveView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CCurveSheet m_CurveSheet;
	//{{AFX_MSG(CCurveView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURVEVIEW_H__CECC7332_2E4B_4146_93F0_2A660B3DC5AA__INCLUDED_)
