#if !defined(AFX_RIGHTVIEW_H__6AB2B5D0_6496_4CCD_8F96_EE3DA09C795B__INCLUDED_)
#define AFX_RIGHTVIEW_H__6AB2B5D0_6496_4CCD_8F96_EE3DA09C795B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RightView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRightView dialog

class CRightView : public CDialog
{
// Construction
public:
	CRightView(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRightView)
	enum { IDD = IDD_FORMVIEWINFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRightView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRightView)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIGHTVIEW_H__6AB2B5D0_6496_4CCD_8F96_EE3DA09C795B__INCLUDED_)
