#if !defined(AFX_INFOVIEW_H__6AF2E976_9610_4C9C_A190_87964BAD59CF__INCLUDED_)
#define AFX_INFOVIEW_H__6AF2E976_9610_4C9C_A190_87964BAD59CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoView.h : header file
//
//#include "Info.h"

/////////////////////////////////////////////////////////////////////////////
// CInfoView view

class CInfoView : public CListView
{
protected:
	CInfoView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInfoView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CInfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CInfoView)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOVIEW_H__6AF2E976_9610_4C9C_A190_87964BAD59CF__INCLUDED_)
