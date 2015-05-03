// InteractViewer.cpp : implementation file
//

#include "stdafx.h"
#include "InteractViewer.h"


// InteractViewer

IMPLEMENT_DYNAMIC(InteractViewer, CStatic)

InteractViewer::InteractViewer()
: m_cbFunc(NULL), m_pUserData(NULL)
, m_funcOnPaint(NULL), m_pUserDataOnPaint(NULL)
, m_nId(-1), m_bActived(false), m_bInUse(false)
{

}

InteractViewer::~InteractViewer()
{
}


BEGIN_MESSAGE_MAP(InteractViewer, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void InteractViewer::iSetEvent(IV_EVENT eEvent, int x, int y)
{
	if(m_cbFunc)
	{
		m_cbFunc(eEvent, x, y, m_pUserData, m_nId);
	}
}
// InteractViewer message handlers

void InteractViewer::OnLButtonDown(UINT nFlags, CPoint point)
{
	iSetEvent(IV_EVENT_L_BTN_DOWN, point.x, point.y);

	CStatic::OnLButtonDown(nFlags, point);
}


void InteractViewer::OnLButtonUp(UINT nFlags, CPoint point)
{
	iSetEvent(IV_EVENT_L_BTN_UP, point.x, point.y);

	CStatic::OnLButtonUp(nFlags, point);
}

void InteractViewer::OnMouseMove(UINT nFlags, CPoint point)
{
	iSetEvent(IV_ENVET_MOUSE_MOVE, point.x, point.y);

	CStatic::OnMouseMove(nFlags, point);
}

void InteractViewer::CleanWindow()
{
	CPaintDC dc(this);

	RECT tRect;
	GetClientRect(&tRect);
	dc.SelectStockObject(GRAY_BRUSH);
	dc.Rectangle(&tRect);
}

void InteractViewer::OnLButtonDblClk(UINT nFlags, CPoint point)
{
//	::SendMessage(GetParent()->m_hWnd, WM_USER_ENLARGE_VIEW, 0, 0);
	CStatic::OnLButtonDblClk(nFlags, point);
}

void InteractViewer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages

	RECT tRect;
	GetClientRect(&tRect);
	tRect.right += 3;
	tRect.bottom += 3;

	CDC *pDC = GetWindowDC();
	
	if(m_bInUse == false)
	{
		pDC->SelectStockObject(GRAY_BRUSH);
		pDC->Rectangle(&tRect);
	}
	
	if(m_funcOnPaint)
		m_funcOnPaint(m_nId, m_bActived, m_bInUse, m_pUserDataOnPaint);

	if(m_bActived)
		pDC->Draw3dRect(&tRect, RGB(0, 255, 0), RGB(0, 255, 0));
	else
		pDC->Draw3dRect(&tRect, RGB(0, 0, 0), RGB(0, 0, 0));
	ReleaseDC(pDC);
}
