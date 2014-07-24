// SplitterWndEx.cpp: implementation of the CSplitterWndEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GdiLeakDetector.h"
#include "SplitterWndEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSplitterWndEx, CSplitterWnd)
	//{{AFX_MSG_MAP(CSplitterWndEx)
		ON_WM_LBUTTONDOWN()
		ON_WM_SETCURSOR()
		ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CSplitterWndEx::CSplitterWndEx()
{

}

CSplitterWndEx::~CSplitterWndEx()
{

}


void CSplitterWndEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    CWnd::OnLButtonDown(nFlags, point);
}

BOOL CSplitterWndEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    // TODO: Add your message handler code here and/or call default
    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CSplitterWndEx::OnMouseMove(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    
    CWnd::OnMouseMove(nFlags, point);
}
