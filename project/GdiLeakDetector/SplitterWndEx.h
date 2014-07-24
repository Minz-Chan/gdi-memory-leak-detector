// SplitterWndEx.h: interface for the CSplitterWndEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPLITTERWNDEX_H__202B8961_AD79_4894_9866_F1325B3D613A__INCLUDED_)
#define AFX_SPLITTERWNDEX_H__202B8961_AD79_4894_9866_F1325B3D613A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSplitterWndEx : public CSplitterWnd  
{
public:
	CSplitterWndEx();
	virtual ~CSplitterWndEx();

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);  
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);  
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);  
	
	
	DECLARE_MESSAGE_MAP() 

};

#endif // !defined(AFX_SPLITTERWNDEX_H__202B8961_AD79_4894_9866_F1325B3D613A__INCLUDED_)
