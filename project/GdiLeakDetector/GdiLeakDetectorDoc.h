// GdiLeakDetectorDoc.h : interface of the CGdiLeakDetectorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDILEAKDETECTORDOC_H__2B87AFA8_5219_49F4_B329_923DB897D5DB__INCLUDED_)
#define AFX_GDILEAKDETECTORDOC_H__2B87AFA8_5219_49F4_B329_923DB897D5DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGdiLeakDetectorDoc : public CDocument
{
protected: // create from serialization only
	CGdiLeakDetectorDoc();
	DECLARE_DYNCREATE(CGdiLeakDetectorDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGdiLeakDetectorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGdiLeakDetectorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGdiLeakDetectorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDILEAKDETECTORDOC_H__2B87AFA8_5219_49F4_B329_923DB897D5DB__INCLUDED_)
