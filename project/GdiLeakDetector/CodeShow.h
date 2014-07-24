#if !defined(AFX_CODESHOW_H__FD113076_6912_4101_97AD_9E3B96033AF0__INCLUDED_)
#define AFX_CODESHOW_H__FD113076_6912_4101_97AD_9E3B96033AF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CodeShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCodeShow dialog

class CCodeShow : public CDialog
{
// Construction
public:
	void SetFileName(CString szFileName);
	void SetLineNum(int nLine);
	CCodeShow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCodeShow)
	enum { IDD = IDD_SHOWCODE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nLineNum;
	CString m_szFileName;

	// Generated message map functions
	//{{AFX_MSG(CCodeShow)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODESHOW_H__FD113076_6912_4101_97AD_9E3B96033AF0__INCLUDED_)
