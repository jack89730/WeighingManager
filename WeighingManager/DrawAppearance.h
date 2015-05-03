//////// DrawAppearance  .h: interface for the CDrawAppearance   class.
//
//////////////////////////////////////////////////////////////////////
/*
			|**************************************************|
			| 雄鹰的翅膀只有在凌空翱翔的时候才最美丽 -- Tercel |
			|--------------------------------------------------|   
			|      email：ChinaTianXiang@foxmail.com		   |                                                         
			|**************************************************|
*/
#if !defined(AFX_DRAWAPPEARANCE_H__D181D9FE_00B4_42F3_9432_37A4027770DD__INCLUDED_)
#define AFX_DRAWAPPEARANCE_H__D181D9FE_00B4_42F3_9432_37A4027770DD__INCLUDED_

#include "AFXWIN.H"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDrawAppearance 
{
public:
	CDrawAppearance  ();
	virtual ~CDrawAppearance  (); 
private :
	WORD  m_wdFontH;			//标题字体高度	：0
	WORD  m_wdFontW;			//标题字体宽度	：0	
	WORD  m_wdSpace;			//控制系统按钮的距离：1
	
	UINT  m_unIDMInNor;			//系统按钮正常状态时的位图ID  在DrawStytemBtn函数中保存 ：0
	UINT  m_unIDMaxNor;
	UINT  m_unIDResNor;
	UINT  m_unIDClsNor;	
	UINT  m_unIDMInDown;		//系统按钮按下状态时的位图ID  在NcLButtonDown函数中保存 ：0
	UINT  m_unIDMaxDown;
	UINT  m_unIDResDown;
	UINT  m_unIDClsDown;
	
	
	BOOL  m_bRedrawWnd;			//标示整个窗口是否发生重绘，在DrawBtnBmp函数中判断 ：FALSE
	BOOL  m_bDownMin;			//标示按钮是否处于按下状态 
	BOOL  m_bDownMax;
	BOOL  m_bDownCls;
	
	CRect m_rtMinBtn;			//保存系统按钮的位置 ：NULL
	CRect m_rtMaxBtn;
	CRect m_rtCloseBtn;	
	CRect m_rtHelpBtn;	//帮助按钮位置
	CRect m_rtIcon;		//图标位置

	COLORREF m_BKColor;
	CBrush   m_BKBrush;	
	COLORREF m_clrTitle;		//标题颜色 ：RGB(255,255,255)
protected:
	void LButtonUP(HWND hWnd, CPoint point);
	
	void NcMouseMove(HWND hWnd, CPoint point, UINT nIDClose, UINT nIDMax, UINT nIDRestore, UINT nIDMin);  

	void NcMouseMove1(HWND hWnd, CPoint point,UINT unIDClose, UINT unIDMax, UINT unIDRestore, UINT unIDMin);
	
	void NcLButtonDblClk(HWND hWnd, CPoint point);
	
	BOOL NcLButtonDown(HWND hWnd, CPoint point, UINT nIDClose, UINT nIDMax, UINT nIDRestore, UINT nIDMin);

	BOOL NcLButtonDown1(HWND hWnd, CPoint point, UINT unIDClose, UINT unIDMax, UINT unIDRestore, UINT unIDMin);
	
	BOOL NcLButtonUp(HWND hWnd, CPoint point);
	
	BOOL TransparentBlt(HDC hdcDest,int nXOriginDest,int nYOriginDest,int nWidthDest,int nHeightDest,HDC hdcSrc,
			int nXOriginSrc,int nYOriginSrc,int nWidthSrc,int nHeightSrc,UINT crTransparent);	
	
	CRect GetTitleBarRect(CDC *pWinDC);											//获得标题栏矩形
	
	CRect DrawBtnBmp(CDC *pWinDC, UINT nIDBmp, CRect rect=NULL);					
public:
	void SetTitleFontSize(WORD nFontW,WORD nFontH);								//设置标题字体
	
	void SetTitleColor(COLORREF clrTitle);										//设置标题颜色

	BOOL DrawIcon(CDC *pWinDC,UINT idIcon,int nLeftSpace);					
	
	BOOL DrawTitle(CDC *pWinDC,CString strTitle,int nLeftSpace);				//绘制窗口标题
	
	BOOL DrawTitleBar(CDC *pWinDC,COLORREF clrTop,COLORREF clrBottom);			//绘制标题栏
	
	BOOL DrawBorder(CDC *pWinDC,COLORREF color,int nLefOffset=0,int nTopOffset=0,int nRitOffset=0,int nBtmOffset=0);
	
	BOOL InterceptMessage(UINT msg,HWND hWnd,CPoint point,UINT nIDClose=0,UINT nIDMax=0,UINT nIDRestore=0,UINT nIDMin=0);

	BOOL InterceptMessage(UINT msg, HWND hWnd,WPARAM wParam, LPARAM lParam);
	
	BOOL DrawSystemBtn(CDC *pWinDC,UINT nIDClose=0,UINT nIDMax=0,UINT nIDRestore=0,UINT nIDMin=0);
	
	BOOL DrawBackground(CDC *pWinDC,CRect *pDesRect,COLORREF clrBrush,UINT nIDBitmap,BOOL bCenter=FALSE,
			BOOL bTransparent=FALSE,COLORREF clrTransparent=0);	
	void DrawDlg(CDC *pWinDC, COLORREF colorValue);//背景色填充
	void DrawDlg(CDC *pWinDC,UINT LeftBmp,UINT RightBmp,UINT ButtomBmp,UINT TitleBmp);
	void DrawTitleBar(CDC *pWinDC,COLORREF color);
	void DrawSystemBtn1(CDC *pWinDC,UINT nIDSysBtn);
};

#endif // !defined(AFX_DRAWAPPEARANCE_H__D181D9FE_00B4_42F3_9432_37A4027770DD__INCLUDED_)
