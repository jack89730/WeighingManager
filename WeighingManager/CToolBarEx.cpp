#include "stdafx.h"
#include "CToolBarEx.h"

BEGIN_MESSAGE_MAP(CToolBarEx, CToolBar)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CToolBarEx::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//CBitmap m_bk;
	//m_bk.LoadBitmap(IDB_BITMAP_BG);
	//CDC m_dcbk;
	//m_dcbk.CreateCompatibleDC(pDC);
	//BITMAP m_bkInfo;

	//m_bk.GetBitmap(&m_bkInfo);

	//m_dcbk.SelectObject(&m_bk);

	//CRect rect;

	//GetClientRect(&rect);

	//pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&m_dcbk,0,0,m_bkInfo.bmWidth,/*m_bkInfo.bmHeight*/rect.Height(),SRCCOPY); 
	
	CBrush brush(RGB(160, 200, 235));
	CRect rect;
    GetClientRect(&rect);
	pDC->FillRect(&rect, &brush);
	return 1;
}

