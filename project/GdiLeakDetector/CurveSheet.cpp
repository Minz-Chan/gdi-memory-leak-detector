// CurveSheet.cpp : implementation file
//

#include "stdafx.h"
#include "gdileakdetector.h"
#include "CurveSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurveSheet

HWND CCurveSheet::hWnd = 0;

IMPLEMENT_DYNAMIC(CCurveSheet, CPropertySheet)

CCurveSheet::CCurveSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CCurveSheet::CCurveSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	// ÃÌº” Ù–‘“≥
	AddPage(&m_Cpu);
	AddPage(&m_Memory);
	AddPage(&m_Thread);
	AddPage(&m_GdiNumber);
	AddPage(&m_CategoryStatistics);
	AddPage(&m_ObjWatch);
	AddPage(&m_Output);
	
}

CCurveSheet::~CCurveSheet()
{
}


BEGIN_MESSAGE_MAP(CCurveSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CCurveSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurveSheet message handlers
