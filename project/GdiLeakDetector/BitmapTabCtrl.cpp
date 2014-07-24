// BitmapTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BitmapTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitmapTabCtrl

CBitmapTabCtrl::CBitmapTabCtrl(){

}

CBitmapTabCtrl::~CBitmapTabCtrl(){

}


BEGIN_MESSAGE_MAP(CBitmapTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CBitmapTabCtrl)
		ON_WM_PAINT()
		ON_WM_ERASEBKGND()
	 //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapTabCtrl message handlers


BOOL CBitmapTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	if(m_bmp.GetSafeHandle() && m_nType)
	{
		CRect rect;
		GetClientRect(rect);

		CDC bgDC;
		bgDC.CreateCompatibleDC(NULL);
		bgDC.SelectObject(&m_bmp);

		BITMAP bmp;
		m_bmp.GetBitmap(&bmp);
		int nWidth = bmp.bmWidth;
		int nHeight = bmp.bmHeight;

		switch(m_nType)
		{
			case BITMAP_CENTER:
				CTabCtrl::OnEraseBkgnd(pDC);
				BITMAP bmp;
				m_bmp.GetBitmap (&bmp);
				pDC->BitBlt ((rect.Width() - bmp.bmWidth) / 2, (rect.Height() - bmp.bmHeight) / 2, bmp.bmWidth, bmp.bmHeight, &bgDC, 0, 0, SRCCOPY);
				return TRUE;
			case BITMAP_STRETCH:
				pDC->SetStretchBltMode(HALFTONE);
				pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &bgDC, 0, 0, nWidth, nHeight, SRCCOPY);
				return TRUE;
			default:
				CBrush bgBrush(&m_bmp);
				pDC->FillRect(rect,&bgBrush);
				return TRUE;
		}
	}
	else
	CTabCtrl::OnEraseBkgnd(pDC);


	return TRUE; //Let's get rid of flickering
}


void CBitmapTabCtrl::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	CRect focusRect;
	CRect activeRect;


	//选择字体
	NONCLIENTMETRICS nclim;
	nclim.cbSize=sizeof(NONCLIENTMETRICS);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
	sizeof(NONCLIENTMETRICS),
	&nclim,0);
	HFONT hMenuFont= ::CreateFontIndirect(&nclim.lfMenuFont);
	::SelectObject(dc.GetSafeHdc(), hMenuFont);


	//绘制文字
	char buf[256];
	TCITEM tcitem;
	tcitem.mask=TCIF_TEXT;
	tcitem.pszText =buf;
	tcitem.cchTextMax=sizeof(buf);

	for(int i=0; i<GetItemCount(); i++)
	{
		memset(buf, 0, sizeof(buf));
		GetItemRect(i,&rect);
		GetItem(i,&tcitem);

		if(GetCurSel() == i)
		{
			rect.top -= 2;
			rect.bottom -=2;
			focusRect.top = rect.top + 2;
			focusRect.bottom = rect.bottom -1;
			focusRect.left = rect.left + 2;
			focusRect.right = rect.right - 2;
			::SetBkMode(dc.GetSafeHdc(), OPAQUE);
			::DrawFocusRect(dc.GetSafeHdc(),&focusRect);
		}

		SetBkMode(dc.GetSafeHdc(), TRANSPARENT);

		SetTextColor(dc.GetSafeHdc(), RGB(0,0,255));//根据需要选择字体颜色
		::DrawText(dc.GetSafeHdc(), buf, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	}

	//绘制线条
	CPen lightPen;
	CPen darkPen;
	COLORREF color;
	color = GetSysColor(COLOR_BTNHIGHLIGHT);
	lightPen.CreatePen(PS_SOLID, 1, color);
	color = GetSysColor(COLOR_3DSHADOW);
	darkPen.CreatePen(PS_SOLID, 1, color);

	for(i=0; i<GetItemCount(); i++)
	{
		GetItemRect(i,&rect);
		//如果是当前项
		if(i == GetCurSel())
		{
		rect.top -= 2;
		rect.left -=2;
		rect.right +=1;
		}

		dc.SelectObject(lightPen);
		dc.MoveTo(rect.left,rect.bottom);

		//画左面
		if(i - 1 == GetCurSel())
		{
		dc.MoveTo(rect.left + 4, rect.top);
		}
		else
		{
		dc.LineTo(rect.left, rect.top + 2);
		dc.LineTo(rect.left + 2, rect.top);
		}

		//画上面
		dc.LineTo(rect.right - 1, rect.top);

		//画右面
		dc.SelectObject(darkPen);
		//后一项被选中
		if(i + 1 != GetCurSel())
		{
			dc.LineTo(rect.right - 1, rect.bottom);
			dc.MoveTo(rect.right, rect.top + 1);
			dc.LineTo(rect.right, rect.bottom);
		}
	}

	//画下面
	GetClientRect(&rect);
	GetItemRect(GetCurSel(),&activeRect);
	dc.SelectObject(lightPen);
	dc.MoveTo(rect.left, activeRect.bottom);
	dc.LineTo(activeRect.left -2, activeRect.bottom);
	dc.MoveTo(activeRect.right, activeRect.bottom);
	dc.LineTo(rect.right, activeRect.bottom);

	CPen apen;
	apen.CreatePen(PS_SOLID, 1, RGB(222,2,2));
	//dc.SelectObject(apen);

	dc.MoveTo(rect.left, activeRect.bottom);
	dc.LineTo(rect.left, rect.bottom - 1);
	dc.LineTo(rect.right - 1, rect.bottom - 1);
	dc.LineTo(rect.right - 1, activeRect.bottom);

}
 

void CBitmapTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS){
	

}

VOID CBitmapTabCtrl::DrawItemText(){

}

VOID CBitmapTabCtrl::DrawItemEdge(){

}

BOOL CBitmapTabCtrl::SetBitmap(UINT uResource, int nType)
{
	m_nType = nType;
	ASSERT(m_nType == BITMAP_TILE || m_nType == BITMAP_STRETCH || m_nType == BITMAP_CENTER);
	BOOL res = m_bmp.LoadBitmap(uResource);
	return res;
}
