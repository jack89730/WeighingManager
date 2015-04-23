// DrawAppearance  .cpp: interface for the CDrawAppearance   class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DrawAppearance.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDrawAppearance ::CDrawAppearance ()
{
	m_clrTitle=RGB(255,255,255);
	m_wdFontW = 0;
	m_wdFontH = 0;
	m_wdSpace = 1;
	
	m_rtMaxBtn=NULL;
	m_rtMinBtn=NULL;
	m_rtCloseBtn=NULL;

	m_unIDMInNor=0;
	m_unIDClsNor=0;
	m_unIDMaxNor=0;
	m_unIDResNor=0;

	m_unIDMInDown=0;
	m_unIDClsDown=0;
	m_unIDMaxDown=0;
	m_unIDResDown=0;
	
	m_bDownMin=FALSE;
	m_bDownMax=FALSE;
	m_bDownCls=FALSE;
	
	m_bRedrawWnd=FALSE;
}

CDrawAppearance ::~CDrawAppearance ()
{

}

/************************************************************************/
/* 功能：绘制窗口非客户区
/*-------------------------------------------------------------------
/* 参数：CDC	  *pWinDC		---- 窗口 DC    	
/*		 int	  LeftBmp	---- 左 ：0 (调整边框的bmp图片)
/*		 int	  RightBmp	---- 右 ：0
/*		 int	  ButtomBmp	---- 下	：0
/*		 int	  TitleBmp	---- 上	：0
/*-------------------------------------------------------------------
/* 返回：	
/*-------------------------------------------------------------------
/* 注  ：本DC是利用GetWindowDC()获得 不是GetDC()
/*		 
/* GetWindowDC()： 获得整个窗口的DC     GetDC()： 获得客户区域DC
/*
/************************************************************************/

void CDrawAppearance::DrawDlg(CDC *pWinDC,COLORREF colorValue)//背景色填充
{
	ASSERT(pWinDC!=NULL);

	CDC memDC;
	CRect wndRC;
	CBitmap bmp;

	::GetWindowRect(pWinDC->GetWindow()->m_hWnd,&wndRC);	
	wndRC.OffsetRect(wndRC.Width(),wndRC.Height());
	int nTitleHeight = GetSystemMetrics(SM_CYCAPTION);
	int nborder = ::GetSystemMetrics(SM_CXFIXEDFRAME);

	if(pWinDC->GetWindow()->GetStyle()&WS_THICKFRAME)
	{
		int nborder = ::GetSystemMetrics(SM_CXBORDER);
	}

	memDC.CreateCompatibleDC(NULL);

	//加载左边界
	bmp.CreateCompatibleBitmap(pWinDC,nborder,wndRC.Height()-nTitleHeight);
	memDC.SelectObject(&bmp);
	memDC.FillSolidRect(0,0,nborder,wndRC.Height()-nTitleHeight, colorValue);
	pWinDC->BitBlt(0,nTitleHeight,nborder,wndRC.Height()-nTitleHeight,&memDC,0,0,SRCCOPY);
	bmp.DeleteObject();

	//加载右边界
	bmp.CreateCompatibleBitmap(pWinDC,nborder,wndRC.Height()-nTitleHeight);
	memDC.SelectObject(&bmp);
	memDC.FillSolidRect(0,0,nborder,wndRC.Height()-nTitleHeight,colorValue);
	pWinDC->BitBlt(wndRC.Width()-nborder,nTitleHeight,nborder,wndRC.Height()-nTitleHeight,&memDC,0,0,SRCCOPY);
	bmp.DeleteObject();

	//加载底边
	bmp.CreateCompatibleBitmap(pWinDC,wndRC.Width()-2*nborder,nborder);
	memDC.SelectObject(&bmp);
	memDC.FillSolidRect(0,0,wndRC.Width()-2*nborder,nborder,colorValue);
	pWinDC->BitBlt(nborder,wndRC.Height()-nborder,wndRC.Width()-2*nborder,wndRC.Height(),&memDC,0,0,SRCCOPY);
	bmp.DeleteObject();


	////加载标题栏
	bmp.CreateCompatibleBitmap(pWinDC,wndRC.Width()-2*nborder,nborder);
	memDC.SelectObject(&bmp);
	memDC.FillSolidRect(0,0,wndRC.Width(),nTitleHeight+nborder,colorValue);
	pWinDC->BitBlt(0,0,wndRC.Width(),nTitleHeight+nborder,&memDC,0,0,SRCCOPY);

	bmp.DeleteObject();
	memDC.DeleteDC();
} 

/************************************************************************/
/* 功能：绘制窗口非客户区
/*-------------------------------------------------------------------
/* 参数：CDC	  *pWinDC		---- 窗口 DC    
/*		 COLORREF color			---- 边框颜色	
/*-------------------------------------------------------------------
/* 返回：	
/*-------------------------------------------------------------------
/* 注  ：本DC是利用GetWindowDC()获得 不是GetDC()
/*		 
/* GetWindowDC()： 获得整个窗口的DC     GetDC()： 获得客户区域DC
/*
/************************************************************************/
void CDrawAppearance::DrawDlg(CDC *pWinDC,UINT LeftBmp,UINT RightBmp,UINT ButtomBmp,UINT TitleBmp)
{
	ASSERT(pWinDC!=NULL);

	CDC memDC;
	CRect wndRC;
	CBitmap bmp;

	::GetWindowRect(pWinDC->GetWindow()->m_hWnd,&wndRC);
	wndRC.OffsetRect(wndRC.Width(),wndRC.Height());
	int nTitleHeight = GetSystemMetrics(SM_CYCAPTION);
	int nborder = ::GetSystemMetrics(SM_CXFIXEDFRAME);
	if(pWinDC->GetWindow()->GetStyle()&WS_THICKFRAME)
	{
		int nborder = ::GetSystemMetrics(SM_CXBORDER);
	}

	//加载左边界位图
	bmp.LoadBitmap(LeftBmp);
	BITMAP so_bmp;
	bmp.GetBitmap(&so_bmp);
	int nBmpWidth, nbmpHeight;
	nBmpWidth = so_bmp.bmWidth;
	nbmpHeight = so_bmp.bmHeight;

	memDC.CreateCompatibleDC(pWinDC);
	memDC.SelectObject(&bmp);
	pWinDC->StretchBlt(0,nTitleHeight,nborder,wndRC.Height()-nTitleHeight,&memDC,0,0,nBmpWidth,nbmpHeight,SRCCOPY);
	bmp.DeleteObject();

	//加载右边界位图
	bmp.LoadBitmap(RightBmp);
	bmp.GetBitmap(&so_bmp);
	nBmpWidth = so_bmp.bmWidth;
	nbmpHeight = so_bmp.bmHeight;

	memDC.SelectObject(&bmp);
	pWinDC->StretchBlt(wndRC.Width()-nborder,nTitleHeight,nborder,wndRC.Height()-nTitleHeight,&memDC,0,0,nBmpWidth,nbmpHeight,SRCCOPY);
	bmp.DeleteObject();

	//加载底边位图
	bmp.LoadBitmap(ButtomBmp);
	bmp.GetBitmap(&so_bmp);
	nBmpWidth = so_bmp.bmWidth;
	nbmpHeight = so_bmp.bmHeight;

	memDC.SelectObject(&bmp);
	pWinDC->StretchBlt(nborder,wndRC.Height()-nborder,wndRC.Width()-2*nborder,nborder,&memDC,0,0,nBmpWidth,nbmpHeight,SRCCOPY);
	bmp.DeleteObject();


	//加载标题栏位图
	bmp.LoadBitmap(TitleBmp);
	bmp.GetBitmap(&so_bmp);
	nBmpWidth = so_bmp.bmWidth;
	nbmpHeight = so_bmp.bmHeight;

	memDC.SelectObject(&bmp);
	pWinDC->StretchBlt(0,0,wndRC.Width(),nTitleHeight+nborder,&memDC,0,0,nBmpWidth,nbmpHeight,SRCCOPY);
	bmp.DeleteObject();

	memDC.DeleteDC();
} 


/************************************************************************/
/* 功能：绘制标题栏
/*------------------------------------------------
/* 参数：CDC	  *pWinDC		---- 窗口DC
		 COLORREF clrTop		---- 标题栏顶部颜色	
		 COLORREF clrBottom		---- 标题栏底部颜色
/*------------------------------------------------		
/* 返回：成功返回 TRUE   DC无效返回FALSE
/*------------------------------------------------
/* 注  ：本DC是利用GetWindowDC()获得 不是GetDC()
/************************************************************************/
void CDrawAppearance::DrawTitleBar(CDC *pWinDC, COLORREF color)
{
	CDC memDC;
	CBitmap bmp;
	CRect rtWnd, rtTitle;
	CPoint point;
	::GetWindowRect(pWinDC->GetWindow()->m_hWnd,&rtWnd);

	//获取标题栏大小
	rtTitle.left = GetSystemMetrics(SM_CXFRAME);
	rtTitle.top = GetSystemMetrics(SM_CYFRAME);
	rtTitle.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXFRAME);
	rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);

	point.x = rtWnd.right  + GetSystemMetrics(SM_CXFRAME);
	point.y = 30;

	memDC.CreateCompatibleDC(pWinDC);
	bmp.CreateCompatibleBitmap(pWinDC,point.x,point.y);
	memDC.SelectObject(&bmp);

	memDC.FillSolidRect(0,0,point.x,point.y, color);	//背景颜色

	////贴图
	/*CBitmap pic;
	pic.LoadBitmap(IDB_BITMAP_BG);
	ShowBitmap(&memDC,0,0, rtTitle.right, 30, pic);*/

	pWinDC->BitBlt(0,0,point.x,point.y,&memDC,0,0,SRCCOPY);
	
	memDC.DeleteDC();
	bmp.DeleteObject();
}

void CDrawAppearance::DrawSystemBtn1(CDC *pWinDC,UINT nIDSysBtn)
{
	CBrush Brush(RGB(0,100,255));//RGB(165,201,235)
	CBrush* pOldBrush = pWinDC->SelectObject(&Brush);

	CRect rtWnd, rtTitle, rtButtons;
	::GetWindowRect(pWinDC->GetWindow()->m_hWnd,&rtWnd);

	//取得标题栏的位置
	rtTitle.left = GetSystemMetrics(SM_CXFRAME);
	rtTitle.top = GetSystemMetrics(SM_CYFRAME);
	rtTitle.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXFRAME);
	rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);

	//计算最小化按钮的位置，位图大小为15X15
	rtButtons.left = rtTitle.right-55;
	rtButtons.top= rtTitle.top;
	rtButtons.right = rtTitle.right-37;
	rtButtons.bottom = rtTitle.top+20;
	m_rtMinBtn = rtButtons;
	//计算最大化按钮的位置，位图大小为15X15
	m_rtMaxBtn.left = m_rtMinBtn.left + 18;
	m_rtMaxBtn.top = m_rtMinBtn.top;
	m_rtMaxBtn.right = m_rtMinBtn.right + 16;
	m_rtMaxBtn.bottom = m_rtMinBtn.bottom;

	m_rtCloseBtn.left = m_rtMaxBtn.left + 18;
	m_rtCloseBtn.top =  m_rtMaxBtn.top;
	m_rtCloseBtn.right = m_rtMaxBtn.right + 16;
	m_rtCloseBtn.bottom = m_rtMaxBtn.bottom;

	CBitmap* pBitmap = new CBitmap;
	BITMAP BmpInfo;
	CBitmap* pOldBitmap;
	CDC* pDisplayMemDC=new CDC;
	pDisplayMemDC->CreateCompatibleDC(pWinDC);

	//rtButtons = m_rtMinBtn;
	pBitmap->LoadBitmap(nIDSysBtn);
	pBitmap->GetBitmap(&BmpInfo);
	pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
	pWinDC->BitBlt(rtButtons.left, rtButtons.top-6, BmpInfo.bmWidth, BmpInfo.bmHeight, pDisplayMemDC, 0, 0, SRCCOPY);
	pDisplayMemDC->SelectObject(pOldBitmap);

	pBitmap->DeleteObject();
	pDisplayMemDC->DeleteDC();

}

/************************************************************************/
/* 功能：绘制窗口边框
/*-------------------------------------------------------------------
/* 参数：CDC	  *pWinDC		---- 窗口 DC    
/*		 COLORREF color			---- 边框颜色	
/*		 int	  nLefOffset	---- 左 ：0 (调整边框的粗细 +n 或 -n)
/*		 int	  nTopOffset	---- 上 ：0
/*		 int	  nRitOffset	---- 右	：0
/*		 int	  nBtmOffset	---- 下	：0
/*-------------------------------------------------------------------
/* 返回：成功返回 TRUE   DC无效返回FALSE	
/*-------------------------------------------------------------------
/* 注  ：本DC是利用GetWindowDC()获得 不是GetDC()
/*		 
/* GetWindowDC()： 获得整个窗口的DC     GetDC()： 获得客户区域DC
/*
/************************************************************************/
BOOL CDrawAppearance ::DrawBorder(CDC *pWinDC,COLORREF color,int nLefOffset,int nTopOffset,int nRitOffset,int nBtmOffset)
{
	if(!pWinDC)
	{	//AfxMessageBox("窗口不存在！",MB_ICONASTERISK);
		return FALSE; 
	}

	CBrush Brush(color);								// 创建一个画刷对象
    CBrush* pOldBrush = pWinDC->SelectObject(&Brush);	// 载入指定颜色的画刷
	
    CRect rtWnd;
    pWinDC->GetWindow()->GetWindowRect(&rtWnd);			//获得窗口矩形

    CPoint point;
    //填充顶部框架
    point.x = rtWnd.Width();
	
	//GetSystemMetrics函数来获得非客户区的不同部分的大小，如标题条、菜单和滚动条
	//offset中文意思为偏移量，初始为0
	
	//SM_CXFRAME 窗口边框的边缘宽度（像素）
	//SM_CYFRAME 窗口边框的边缘高度
	//SM_CXSIZE 窗口标题栏宽度
	//SM_CYSIZE 窗口标题栏高度	
	//SM_CXSIZEFRAME、SM_CYFRAME等同于SM_CXSIZE、SM_CYSIZE

    point.y = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYSIZE) + nTopOffset;
	
	//PatBlt在设备上创建模式,将设备已有的模式与选择的画刷组合
	//参数：矩形左上角坐标，长宽，光栅操作类型
    pWinDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
    
	//填充左侧框架
    point.x = GetSystemMetrics(SM_CXFRAME) + nLefOffset;			
    point.y = rtWnd.Height();
    pWinDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
    
	//填充底部框架
    point.x = rtWnd.Width(); 
    point.y = GetSystemMetrics(SM_CYFRAME) + nBtmOffset;
    pWinDC->PatBlt(0, rtWnd.Height()-point.y, point.x, point.y, PATCOPY);
    
	//填充右侧框架
    point.x = GetSystemMetrics(SM_CXFRAME) + nRitOffset;
    point.y = rtWnd.Height();
    pWinDC->PatBlt(rtWnd.Width()-point.x, 0, point.x, point.y, PATCOPY);

	return TRUE;
}

/************************************************************************/
/* 功能：绘制标题栏
/*------------------------------------------------
/* 参数：CDC	  *pWinDC		---- 窗口DC
		 COLORREF clrTop		---- 标题栏顶部颜色	
		 COLORREF clrBottom		---- 标题栏底部颜色
/*------------------------------------------------		
/* 返回：成功返回 TRUE   DC无效返回FALSE
/*------------------------------------------------
/* 注  ：本DC是利用GetWindowDC()获得 不是GetDC()
/************************************************************************/
BOOL CDrawAppearance ::DrawTitleBar(CDC *pWinDC, COLORREF clrTop, COLORREF clrBottom)
{
	if (!pWinDC)
		return FALSE;

	CPen Pen; 
	CRect  rtWnd,rtTitle; 

	::GetWindowRect(pWinDC->GetWindow()->m_hWnd,&rtWnd);		//整个Window的相对于屏幕的矩形 
	rtTitle=GetTitleBarRect(pWinDC);

	rtTitle.left -=10;											//修改尺寸
	rtTitle.top	 -=10;
	rtTitle.right+=10;

	CPen* pOldPen=pWinDC-> SelectObject(&Pen);					//选入画笔

	int R,G,B; 
	R=(GetRValue(clrTop)-GetRValue(clrBottom))/rtTitle.Height();//颜色差值
	G=(GetGValue(clrTop)-GetGValue(clrBottom))/rtTitle.Height(); 
	B=(GetBValue(clrTop)-GetBValue(clrBottom))/rtTitle.Height(); 
	
	int ColR = GetRValue(clrTop),ColG= GetGValue(clrTop),ColB=GetBValue(clrTop); 
	
	for (int i=0;i<rtTitle.Height();i++)					//控制表面的光亮,从矩形的上面开始画横线，一直画 
	{ 
		ColR -= R;											//到矩形的底部，而且颜色逐渐变深 
		ColG -= G; 
		ColB -= B; 

		Pen.DeleteObject(); 
		Pen.CreatePen(PS_SOLID,2,RGB(ColR,ColG,ColB));		//创建画笔
		pWinDC-> SelectObject(&Pen); 

		pWinDC-> MoveTo(rtTitle.left,rtTitle.top+i);		//画线
		pWinDC-> LineTo(rtTitle.right,rtTitle.top+i); 
	} 
	
	pWinDC-> SelectObject(pOldPen);

	return TRUE;
}

/************************************************************************/
/* 功能：绘制窗口图标
/*---------------------------------------------------------
/* 参数：CDC  *pWinDC		---- 窗口DC
		 UINT idIcon		---- 图标ID
		 int  nLeftSpace	---- 图标目标位置与标题栏左边距离
/*----------------------------------------------------------		
/* 返回：成功返回 TRUE   DC无效返回NULL
/*----------------------------------------------------------
/* 注  ：本DC是利用GetWindowDC()获得 不是GetDC()
/************************************************************************/
BOOL CDrawAppearance ::DrawIcon(CDC *pWinDC, UINT idIcon, int nLeftSpace)
{
	if (!pWinDC)
		return NULL;

	CRect rtIcon;										
	CRect rtTitle=GetTitleBarRect(pWinDC);						
	
	rtIcon.left =/*rtTitle.left*/ + nLeftSpace;					//调整图标矩形
	rtIcon.top	=rtTitle.top-5; 
	rtIcon.right=rtIcon.left+rtTitle.bottom-8;
	rtIcon.bottom=rtTitle.bottom+rtIcon.top-10;
								
	::DrawIconEx(pWinDC->m_hDC,rtIcon.left,rtIcon.top, AfxGetApp()->LoadIcon(idIcon), //绘制图标
		rtIcon.Width(), rtIcon.Height(), 0, NULL,DI_NORMAL);
	return TRUE;
}

/************************************************************************/
/* 功能：获得标题栏矩形区域
/************************************************************************/
CRect CDrawAppearance ::GetTitleBarRect(CDC *pWinDC)
{
	if (!pWinDC)
	{	AfxMessageBox("窗口不存在！",MB_ICONASTERISK);
		return NULL; 
	}
	
	CRect rtTitle;
	CRect rtWnd;

	GetWindowRect(pWinDC->GetWindow()->m_hWnd,&rtWnd);		//整个Window的相对于屏幕的矩形 

	rtTitle.left = GetSystemMetrics(SM_CXFRAME); 			//取得整个Title bar的矩形 
	rtTitle.top  = GetSystemMetrics(SM_CYFRAME); 
	rtTitle.right= rtWnd.Width() - GetSystemMetrics(SM_CXFRAME);
	rtTitle.bottom=rtTitle.top + GetSystemMetrics(SM_CYSIZE);
	return rtTitle;
}

/************************************************************************/
/* 功能：绘制窗口标题
/*--------------------------------------------------------
/* 参数：CDC	 *pWinDC	---- 窗口DC
/*		 CString strTitle	---- 标题文本
/*		 int	 nLeftSpace	---- 标题左边与标题栏左边的距离
/*--------------------------------------------------------
/* 返回：成功返回 TRUE   DC无效返回NULL
/*--------------------------------------------------------
/* 注  ：本DC是利用GetWindowDC()获得 不是GetDC()
/************************************************************************/
BOOL CDrawAppearance ::DrawTitle(CDC *pWinDC, CString strTitle, int nLeftSpace)
{	
	if (!pWinDC)
		return FALSE;

	CFont font;										
	CFont *pOldFont;
	
	int nOldMode = pWinDC->SetBkMode(TRANSPARENT);			//文字透明显示
	COLORREF clOldText=pWinDC->SetTextColor(m_clrTitle);	//设置文本颜色
	
	font.CreateFont(m_wdFontH,m_wdFontW,0,0,0,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("楷体_GB2312"));//创建字体
	pOldFont=pWinDC->SelectObject(&font);					//选入字体
	
	//pWinDC->SelectStockObject(OEM_FIXED_FONT);			//选入字体

	CRect rtTitle=GetTitleBarRect(pWinDC);					//获得标题栏矩形

	rtTitle.left += nLeftSpace;								//标题输出位置

	if (m_wdFontH<=0)	
		rtTitle.top -=rtTitle.top/2;
	else
		rtTitle.top +=(rtTitle.Height()-m_wdFontH)/2-1;		//-1是因为有些偏差

	pWinDC->DrawText(strTitle, &rtTitle, DT_LEFT);			//绘制标题
	pWinDC->SetBkMode(nOldMode);							//恢复
	pWinDC->SetTextColor(clOldText);					
	pWinDC->SelectObject(pOldFont);
	
	return TRUE;			
}

/************************************************************************/
/* 功能：绘制窗口背景
/*-----------------------------------------------------------------------
/* 参数：CDC		*pWinDC,	    ---- 窗口DC		
/*		 CRect		*pDesRect,		---- 目标矩形区域
/*		 COLORREF	clrBrush,		---- 背景画刷，为NULL不设置
/*		 UINT		unIDBitmap,		---- 背景位图，为NULL不设置  
/*		 BOOL		bCenter,		---- 是否居中显示，默认TRUE ： FALSE
/*		 BOOL		bTransparent,	---- 位图是否透明显示 ：FALSE
/*		 COLORREF	clrTransparent	---- 将要透明的颜色	  ：0
/*-----------------------------------------------------------------------
/* 返回：bSet	----返回函数是否执行成功	
/*-----------------------------------------------------------------------
/* 注  ：****此DC使用 GetDC()获得*****
/*-----------------------------------------------------------------------				
/* 本函数主要是用于添加文档类背景，如果是对话框修改背景可以在OnCtrlColor中
/*
/* 重新创建画刷 (可以是位图画刷)，但本函数可以实现位图居中显示与透明显示，
/* 
/* 可以更灵活的绘制位图与画刷，但如果透明显示时改变了位图大小，位图会失真。
/* 
/************************************************************************/
BOOL CDrawAppearance ::DrawBackground(CDC *pWinDC, CRect *pDesRect, COLORREF clrBrush, UINT unIDBitmap,
									  BOOL bCenter,BOOL bTransparent,COLORREF clrTransparent)	
{
	BOOL bSet=FALSE;									//返回值		
	if (pWinDC==NULL && !IsWindow(pWinDC->GetWindow()->m_hWnd))
	{	AfxMessageBox("无效的DC,或窗口不存在！",MB_ICONASTERISK);
		return NULL;
	}

	if (clrBrush!=NULL)									//设置背景画刷
	{	
		CBrush brush(clrBrush);							
		CBrush *pOldBrush=pWinDC->SelectObject(&brush);	
		bSet=pWinDC->PatBlt(pDesRect->left,pDesRect->top,pDesRect->Width(),pDesRect->Height(),PATCOPY);
		pWinDC->SelectObject(pOldBrush);					
		pOldBrush->DeleteObject();
	}	
	if (unIDBitmap!=NULL)								//设置背景位图
	{
		BOOL	bZoom=FALSE;
		CDC		dcMem;												
		BITMAP	bmOriginal;
		BITMAP	bmNew;
		CBitmap bitmap;
		int x,y,nWidth,nHeight;
		
		dcMem.CreateCompatibleDC(pWinDC);
		
		if (!bitmap.LoadBitmap(unIDBitmap))				//载入位图
		{
			AfxMessageBox("Load bitmap failure!\r\rThe bitmap exist?",MB_ICONERROR|MB_OK);
			return bSet;
		}
		
		bitmap.GetBitmap(&bmOriginal);					//获得位图尺寸
		CBitmap *pOldBitmap=dcMem.SelectObject(&bitmap);
		
		double dBitmapWidth  = bmOriginal.bmWidth;
		double dBitmapHeight = bmOriginal.bmHeight;
		double dBitmapRatio=dBitmapWidth/dBitmapHeight;	//图像宽高比
		
		if (pDesRect->Width()<bmOriginal.bmWidth)		//如果目标矩形宽小于位图宽
		{	
			bmNew.bmWidth  = (int)(pDesRect->Width()/dBitmapWidth*dBitmapHeight);	
			bmNew.bmHeight = (int)(bmNew.bmWidth/dBitmapRatio);
			bZoom = TRUE;
		}
		else if(pDesRect->Height()<bmOriginal.bmHeight) //如果目标矩形高小于位图高
		{	
			bmNew.bmHeight = (int)(pDesRect->Height()/dBitmapWidth*dBitmapHeight);
			bmNew.bmWidth  = (int)(bmNew.bmHeight*dBitmapRatio);
			bZoom = TRUE;
		}
		else
			bmNew=bmOriginal;							//否则位图大小不变
		
		if (bCenter)
		{	
			x=(pDesRect->Width()-bmNew.bmWidth)/2;		//图像居中显示
			y=(pDesRect->Height()-bmNew.bmHeight)/2;	
			nWidth =bmNew.bmWidth;
			nHeight=bmNew.bmHeight;
		}
		else
		{
			x=pDesRect->left;							//图像填充来整个目标矩形
			y=pDesRect->top;
			nWidth =pDesRect->Width();					
			nHeight=pDesRect->Height();
		}
		
		pWinDC->SetStretchBltMode(HALFTONE);			//设置图像平滑显示 
		if(bTransparent&& !bZoom && bCenter)
			bSet=TransparentBlt(pWinDC->m_hDC,x,y,nWidth,nHeight,dcMem.m_hDC,0,0,bmOriginal.bmWidth,bmOriginal.bmHeight,
							clrTransparent);			//拷贝位图
		else
			bSet=pWinDC->StretchBlt(x,y,nWidth,nHeight,&dcMem,0,0,bmOriginal.bmWidth,bmOriginal.bmHeight,SRCCOPY);//拷贝位图
		
		dcMem.SelectObject(pOldBitmap);					//恢复默位图
		dcMem.DeleteDC();								//释放DC
	}					
	return bSet;
}

/************************************************************************/
/*功能：位图透明显示
/*--------------------------------------------------
/*参数：HDC		hdcDest,		---- 目标DC
/*		int		nXOriginDest,   ---- 目标X偏移
/*		int		nYOriginDest,   ---- 目标Y偏移
/*		int		nWidthDest,     ---- 目标宽度
/*		int		nHeightDest,    ---- 目标高度
/*		HDC		hdcSrc,         ---- 源DC
/*		int		nXOriginSrc,    ---- 源X起点
/*		int		nYOriginSrc,    ---- 源Y起点
/*		int		nWidthSrc,      ---- 源宽度
/*		int		nHeightSrc,     ---- 源高度
/*		UINT	crTransparent   ---- 透明色,COLORREF类型
/*---------------------------------------------------
/*返回：成功返回真 		
/************************************************************************/
BOOL CDrawAppearance ::TransparentBlt( HDC hdcDest,int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,
					   HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, UINT crTransparent)
{
	HBITMAP hOldImageBMP, hImageBMP;
	hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	// 创建兼容位图
	
	HBITMAP hOldMaskBMP, hMaskBMP;
	hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);			// 创建单色掩码位图

	HDC		hImageDC = CreateCompatibleDC(hdcDest);
	HDC		hMaskDC = CreateCompatibleDC(hdcDest);

	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP =  (HBITMAP)SelectObject(hMaskDC, hMaskBMP);

	// 将源DC中的位图拷贝到临时DC中
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, 
					hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);

	// 设置透明色
	SetBkColor(hImageDC, crTransparent);

	// 生成透明区域为白色，其它区域为黑色的掩码位图
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);

	// 生成透明区域为黑色，其它区域保持不变的位图
	SetBkColor(hImageDC, RGB(0,0,0));
	SetTextColor(hImageDC, RGB(255,255,255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// 透明部分保持屏幕不变，其它部分变成黑色
	SetBkColor(hdcDest,RGB(255,255,255));
	SetTextColor(hdcDest,RGB(0,0,0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// "或"运算,生成最终效果
	BOOL bResult=BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);

	// 清理、恢复	
	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
	return bResult;
}

/************************************************************************/
/*功能：设置标题的颜色
/*--------------------
/*注  ：在绘制之前设置
/************************************************************************/
void CDrawAppearance ::SetTitleColor(COLORREF clrTitle)
{
	m_clrTitle=clrTitle;
}

/************************************************************************/
/*功能：设置标题字体大小
/*-----------------------
/*参数：宽度、高度
/*-----------------------
/*注  ：在绘制之前设置
/************************************************************************/
void CDrawAppearance ::SetTitleFontSize(WORD wdFontW, WORD wdFontH)
{
	m_wdFontW=wdFontW;
	m_wdFontH=wdFontH;
}

/************************************************************************/
/* 功能：获得按钮位图ID，调用绘图函数进行绘制，保存按钮位置
/*---------------------------------------------------------
/* 参数：CDC  *pWinDC		---- 窗口DC
/*		 UINT unIDClose 	---- 各个按钮的位图ID 默认值：0
/*		 UINT unIDMax			 
/*		 UINT unIDRestore		 
/*       UINT unIDMin		
/*---------------------------------------------------------
/* 返回：成功返回-TRUE   DC无效返回-NULL
/*---------------------------------------------------------
/* 注  ：本DC是利用GetWindowDC()获得 不是GetDC()
/************************************************************************/
BOOL CDrawAppearance ::DrawSystemBtn(CDC *pWinDC, UINT unIDClose, UINT unIDMax, UINT unIDRestore, UINT unIDMin)
{
	if (!pWinDC)
		return FALSE;
	
	m_bRedrawWnd=TRUE;		

	m_rtCloseBtn = DrawBtnBmp(pWinDC,unIDClose);		//绘制位图并保存位置

	if (pWinDC->GetWindow()->IsZoomed())			
		m_rtMaxBtn = DrawBtnBmp(pWinDC,unIDRestore);
	else
		m_rtMaxBtn = DrawBtnBmp(pWinDC,unIDMax);

	m_rtMinBtn = DrawBtnBmp(pWinDC,unIDMin);

	m_wdSpace = 0;									//全部绘制完毕后要清0

	m_unIDMaxNor = unIDMax;							//保存位图ID
	m_unIDMInNor = unIDMin;
	m_unIDClsNor = unIDClose;
	m_unIDResNor = unIDRestore;

	return TRUE;
}	

/************************************************************************/
/* 功能：绘制按钮位图
/*-----------------------------------------------------------
/* 参数：CDC   *pWinDC		---- 窗口DC
/*		 UINT  unIDBmp 		---- 位图ID
/*		 CRect rect			---- 在MouseMove时,传递保存的位置			
/*-----------------------------------------------------------
/* 返回：成功返回按钮位置   位图ID无效返回-NULL
/*-----------------------------------------------------------
/* 注  ：本DC是利用GetWindowDC()获得 不是GetDC()
/************************************************************************/
CRect CDrawAppearance ::DrawBtnBmp(CDC *pWinDC, UINT unIDBmp, CRect rect)
{
	if (unIDBmp==0)
		return NULL;
	
	CDC		MemDC;
	BITMAP  bmSrc;
	CBitmap	Bitmap,*pOldBitmap;;
	CRect rtButton;
	CRect rtWnd;
	CRect rtTitle=GetTitleBarRect(pWinDC);						//获得标题栏矩形

	MemDC.CreateCompatibleDC(pWinDC);							//创建兼容DC

	Bitmap.LoadBitmap(unIDBmp);									//载入位图
	Bitmap.GetBitmap(&bmSrc);									//获得位图尺寸
	pOldBitmap=MemDC.SelectObject(&Bitmap);						//选入DC

	GetWindowRect(pWinDC->GetWindow()->m_hWnd,&rtWnd);			//整个Window的相对于屏幕的矩形 
	
	if (m_bRedrawWnd)											//如果发生重绘，需要重新计算按钮位置
	{
		if (bmSrc.bmHeight<rtTitle.Height())
		{	
			rtButton.left = rtTitle.right-bmSrc.bmWidth-m_wdSpace;	
			rtButton.right= rtTitle.right-m_wdSpace;
			rtButton.top  = rtTitle.top+(rtTitle.Height()-bmSrc.bmHeight)/2-2;
			rtButton.bottom = rtButton.top+bmSrc.bmHeight;		//设置按钮位置
		}
		else
		{	
			rtButton.bottom = rtTitle.bottom-3;					//调整目标位图大小、位置	
			rtButton.top  = rtTitle.top+1;						//此时就可以得到目标位图高
			
			int nW=bmSrc.bmWidth-(bmSrc.bmHeight-rtButton.Height()); //原位图宽-（原位图高-目标位图高） = 目标位图宽 
		 	
			rtButton.right = rtTitle.right-m_wdSpace;				
			rtButton.left  = rtTitle.right-nW-m_wdSpace;			 	
		}
	}
	else														//未发生重绘，使用保存按钮的位置
	{
		rect.OffsetRect(-rtWnd.left,-rtWnd.top);
		rtButton=rect;
	}
	
	if (bmSrc.bmHeight<rtTitle.Height())
		pWinDC->BitBlt(rtButton.left,rtButton.top,bmSrc.bmWidth,bmSrc.bmHeight,&MemDC,0,0,SRCCOPY);
	else
	{	
		pWinDC->SetStretchBltMode(HALFTONE);					//设置图像平滑显示
		pWinDC->StretchBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), &MemDC, 
			0, 0,bmSrc.bmWidth,bmSrc.bmHeight,SRCCOPY);	
	}
	
	m_wdSpace=rtTitle.right-rtButton.left+1;					//每次加上之前位图的宽度并+1分隔按钮

	MemDC.SelectObject(pOldBitmap);								//选入原始位图
	MemDC.DeleteDC();											//释放DC、资源
	DeleteObject(&Bitmap);

	rtButton.OffsetRect(rtWnd.TopLeft());						//由于rtButton存储的矩形对于窗口区域的
																//而我们要保存的是相对于屏幕的
	return rtButton;											
}

/************************************************************************/
/* 功能：截获消息、并判断消息类型，调用相应函数进行处理
/*---------------------------------------------------------------------
/* 参数：UINT   msg			---- 指定当前截获的消息类型
/*		 HWND   hWnd 		---- 窗口句柄，this->m_hWnd
/*		 CPoint point		---- 消息发生时Mouse所在位置
/*		 UINT   unIDClose	---- 不同消息要传递不同的位图
/*		 UINT   unIDMax			 鼠标移动时传递热点的位图
/*		 UINT   unIDRestore	     鼠标按下时传递按下的位图
/*		 UINT   unIDMin			 4个参数默认值 ： 0
/*----------------------------------------------------------------------
/* 返回：如果传递的消息正确返回 TRUE    否则 FALSE
/*----------------------------------------------------------------------
/* 注  ：msg 可传递MFC提供的标准消息，无需自定义			
/*		 
/*		 在以下列出的对话框的消息响应中添加此函数
/************************************************************************/
BOOL CDrawAppearance ::InterceptMessage(UINT msg, HWND hWnd, CPoint point, UINT unIDClose, 
										UINT unIDMax, UINT unIDRestore, UINT unIDMin)
{	
	BOOL bResult=FALSE;
	m_bRedrawWnd=FALSE;
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		//
		LButtonUP(hWnd,point);
		break;
	case WM_NCMOUSEMOVE:
		//在鼠标移到按钮时，调用绘制命令按钮函数
		//NcMouseMove(hWnd, point, unIDClose, unIDMax, unIDRestore, unIDMin);
		NcMouseMove1(hWnd, point, unIDClose, unIDMax, unIDRestore, unIDMin);
		break;
	case WM_NCLBUTTONDOWN:
		//bResult = NcLButtonDown(hWnd, point, unIDClose, unIDMax, unIDRestore, unIDMin);
		bResult = NcLButtonDown1(hWnd, point, unIDClose, unIDMax, unIDRestore, unIDMin);
		break;
	case WM_NCLBUTTONUP:
		bResult = NcLButtonUp(hWnd, point);
		break;
	case WM_NCLBUTTONDBLCLK:
		NcLButtonDblClk(hWnd, point);
		break;
	default:
		bResult = FALSE;
		break;
	}	

	m_wdSpace=0; //清0 否则影响下一次绘图

	return bResult;
}

/************************************************************************/
/*功能：绘制LButtonDown时的按钮位图
/*------------------------------------------------
/*参数：同上
/*------------------------------------------------		
/*返回：如果鼠标在按钮区域返回 TRUE  否则返回 FALSE (可调用默认函数处理)
/************************************************************************/
BOOL CDrawAppearance ::NcLButtonDown(HWND hWnd, CPoint point, UINT unIDClose, UINT unIDMax, UINT unIDRestore, UINT unIDMin)
{
	m_unIDMInDown = unIDMin;		//保存按下时的位图
	m_unIDClsDown = unIDClose;
	m_unIDMaxDown = unIDMax;
	m_unIDResDown = unIDRestore;

	CDC *pWinDC=CDC::FromHandle(GetWindowDC(hWnd));

	if (m_rtMaxBtn.PtInRect(point))	
	{	
		m_bDownMax = TRUE;
		if (IsZoomed(hWnd))
		{
			DrawBtnBmp (pWinDC, unIDRestore, m_rtMaxBtn);
		}
			
		else
			DrawBtnBmp (pWinDC, unIDMax, m_rtMaxBtn);					
	}
	else if (m_rtCloseBtn.PtInRect(point))	
	{
		m_bDownCls = TRUE;
		DrawBtnBmp (pWinDC, unIDClose, m_rtCloseBtn);
	}
	else if (m_rtMinBtn.PtInRect(point))
	{
		m_bDownMin = TRUE;
		DrawBtnBmp (pWinDC, unIDMin, m_rtMinBtn);
	}
	else	
		return FALSE;

	return TRUE;
}

BOOL CDrawAppearance ::NcLButtonDown1(HWND hWnd, CPoint point, UINT unIDClose, UINT unIDMax, UINT unIDRestore, UINT unIDMin)
{
	m_unIDMInDown = unIDMin;		//保存按下时的位图
	m_unIDClsDown = unIDClose;
	m_unIDMaxDown = unIDMax;
	m_unIDResDown = unIDRestore;
    
	CRect rtWnd;
	GetWindowRect(hWnd,&rtWnd);	

	//mouse坐标转化为本窗口坐标 重要
	point.x = point.x - rtWnd.left;
	point.y = point.y - rtWnd.top;

	CDC *pWinDC=CDC::FromHandle(GetWindowDC(hWnd));

	if (m_rtMaxBtn.PtInRect(point))	
	{	
		m_bDownMax = TRUE;
		if (IsZoomed(hWnd))
		{
			DrawBtnBmp (pWinDC, unIDRestore, m_rtMaxBtn);
		}

		else
			DrawBtnBmp (pWinDC, unIDMax, m_rtMaxBtn);					
	}
	else if (m_rtCloseBtn.PtInRect(point))	
	{
		m_bDownCls = TRUE;
		DrawBtnBmp (pWinDC, unIDClose, m_rtCloseBtn);
	}
	else if (m_rtMinBtn.PtInRect(point))
	{
		m_bDownMin = TRUE;
		DrawBtnBmp (pWinDC, unIDMin, m_rtMinBtn);
	}
	else	
		return FALSE;

	return TRUE;
}


/************************************************************************/
/*功能：双击标题栏大化与恢复窗口
/*------------------------------------------------
/*参数：同上
/************************************************************************/
void CDrawAppearance ::NcLButtonDblClk(HWND hWnd, CPoint point)
{
	if  (IsZoomed(hWnd))
		SendMessage(hWnd,WM_SYSCOMMAND,SC_RESTORE,MAKELPARAM(point.x,point.y));
	else 
	{	
		SendMessage(hWnd,WM_SYSCOMMAND,SC_MAXIMIZE,MAKELPARAM(point.x,point.y));									
	
		CRect rect;
		
		HWND hwTaskbar=FindWindow("Shell_TrayWnd","");		// 获得任务栏窗口句柄			
	
		GetWindowRect(hwTaskbar,&rect);			// 任务栏矩形
		
		int nHeight=rect.Height();  			// 任务栏的高度	
		
		GetWindowRect(hWnd,&rect);

		MoveWindow(hWnd,rect.left,rect.top,rect.Width(),rect.Height()-nHeight,TRUE);//改变窗口高度，否则盖住任务栏	
	};
}

/************************************************************************/
/*功能：鼠标移动到按钮位置绘制热点位图,否则绘制正常状态时位图
/*-----------------------------------------------------------
/*参数：同上
/************************************************************************/
void CDrawAppearance ::NcMouseMove(HWND hWnd, CPoint point,UINT unIDClose, UINT unIDMax, UINT unIDRestore, UINT unIDMin)
{

	CDC *pWinDC= CDC::FromHandle(GetWindowDC(hWnd));			
	BOOL bDown = m_bDownMax || m_bDownMin || m_bDownCls;		// 得出是否有按钮处于按下状态
	 
	if ( m_rtMaxBtn.PtInRect(point) )
	{	
		if( bDown && !m_bDownMax )								// 如果处于按下状态但并不是此按钮返回，如果bDown为FALSE说
				return;											// 明没有按钮被按下，此时m_bDownMax也一定不会是TRUE，那么
		 														// 可以绘制热点状态时的位图，以下同理
		if ( IsZoomed(hWnd) )	
		{	
			if (m_bDownMax)								
				DrawBtnBmp(pWinDC, m_unIDResDown, m_rtMaxBtn);    
			else
				DrawBtnBmp(pWinDC, unIDRestore, m_rtMaxBtn);
		}
		else
		{
			if (m_bDownMax)
				DrawBtnBmp(pWinDC, m_unIDMaxDown, m_rtMaxBtn); 
			else
				DrawBtnBmp(pWinDC, unIDMax, m_rtMaxBtn); 
		}
	}
	else
	{
		if (IsZoomed(hWnd))
			DrawBtnBmp(pWinDC, m_unIDResNor, m_rtMaxBtn);
		else
			DrawBtnBmp(pWinDC, m_unIDMaxNor, m_rtMaxBtn); 
	}
	

	if (m_rtCloseBtn.PtInRect(point))	
	{
		if( bDown && !m_bDownCls )
				return;
		if (m_bDownCls)
			DrawBtnBmp(pWinDC, m_unIDClsDown, m_rtCloseBtn);
		else
			DrawBtnBmp(pWinDC, unIDClose, m_rtCloseBtn);
	}
	else
		DrawBtnBmp(pWinDC, m_unIDClsNor, m_rtCloseBtn);


	if (m_rtMinBtn.PtInRect(point))
	{
		if( bDown && !m_bDownMin )
				return;
		if (m_bDownMin)
			DrawBtnBmp(pWinDC, m_unIDMInDown, m_rtMinBtn);
		else
			DrawBtnBmp(pWinDC, unIDMin, m_rtMinBtn);
	}
	else
		DrawBtnBmp(pWinDC,m_unIDMInNor, m_rtMinBtn);	
}

void CDrawAppearance::NcMouseMove1(HWND hWnd, CPoint point,UINT unIDClose, UINT unIDMax, UINT unIDRestore, UINT unIDMin) 
{
	CDC* pWinDC= CDC::FromHandle(GetWindowDC(hWnd));	
	
	CDC* pDisplayMemDC=new CDC;
	pDisplayMemDC->CreateCompatibleDC(pWinDC);
	CBitmap* pBitmap = new CBitmap;
	CBitmap* pOldBitmap;
	CRect rtWnd, rtButton;

	if (pWinDC)
	{
		CString StrTemp = "";
		GetWindowRect(hWnd,&rtWnd);	
		
		//mouse坐标转化为本窗口坐标 重要
		point.x = point.x - rtWnd.left;
		point.y = point.y - rtWnd.top;
		//判断mouse是否移到系统按钮上
		if (m_rtCloseBtn.PtInRect(point))
		{
			pBitmap->LoadBitmap(unIDClose);
			StrTemp = _T("关闭");
		}
		else
		{
			if(m_rtMinBtn.PtInRect(point))
			{
				pBitmap->LoadBitmap(unIDMin);
				StrTemp = _T("最小化窗口");
			}
			else
			{
				if(m_rtMaxBtn.PtInRect(point))
				{
					pBitmap->LoadBitmap(unIDMax);
					if(IsZoomed(hWnd))
					{
						StrTemp = _T("还原窗口");
					}
					else
					{
						StrTemp = _T("最化大窗口");
					}
				}
				else
				{
					pBitmap->LoadBitmap(unIDRestore);
				}
			}
		}

		rtButton = m_rtMinBtn;
		BITMAP BmpInfo;
		pBitmap->GetBitmap(&BmpInfo);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pWinDC->BitBlt(rtButton.left, rtButton.top-6, BmpInfo.bmWidth, BmpInfo.bmHeight, pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();
		CRect ShowTipRec;
		ShowTipRec = m_rtMinBtn;
		//if(!StrTemp.IsEmpty())
		//{
		//	ScreenToClient(hWnd,&ShowTipRec);
		//	/*m_ToolTip.AddToolTip(IDD_TITLEBAR_DIALOG,&ShowTipRec,StrTemp);
		//	m_ToolTip.SetDelayTime(200);*/
		//}
	}

	pDisplayMemDC->DeleteDC();
	pWinDC->DeleteDC();
	delete pDisplayMemDC;
	delete pBitmap;
}


/************************************************************************/
/*功能：各窗口发送相应消息
/*------------------------------------------------
/*参数：同上
/*------------------------------------------------		
/*返回：如果鼠标在按钮区域返回 TRUE  否则返回 FALSE (可调用默认函数处理)
/************************************************************************/
BOOL CDrawAppearance ::NcLButtonUp(HWND hWnd, CPoint point)
{
	BOOL bDown = m_bDownMax || m_bDownMin || m_bDownCls, bResult=TRUE;	//得出是否有按钮处于按下状态
	
	if ( !bDown )	//没有返回
	{	return FALSE; }

	CRect rtWnd;
	GetWindowRect(hWnd,&rtWnd);	
	//mouse坐标转化为本窗口坐标 重要
	point.x = point.x - rtWnd.left;
	point.y = point.y - rtWnd.top;
 
	CDC *pWinDC=CDC::FromHandle(GetWindowDC(hWnd));						
	
	if ( m_rtMaxBtn.PtInRect(point) && m_bDownMax)						//如果在按钮范围而且之前有被按下
	{	
		if (IsZoomed(hWnd))
			SendMessage(hWnd,WM_SYSCOMMAND,SC_RESTORE,MAKELPARAM(point.x,point.y));
		else
		{	
			SendMessage(hWnd,WM_SYSCOMMAND,SC_MAXIMIZE,MAKELPARAM(point.x,point.y));

			//CRect rtWnd;	
			//CRect rtTask;		
			//
			//HWND hwTaskbar=FindWindow(L"Shell_TrayWnd",L"");				//获得任务栏窗口句柄
			//
			//GetWindowRect(hwTaskbar,&rtTask);							//任务栏矩形				
			//GetWindowRect(hWnd,&rtWnd);									//窗口矩形				
			//TRACE("%d",rtTask.Height());
			////改变窗口高度，否则盖住任务栏
			//SetWindowPos(hWnd,NULL,0,0,rtWnd.Width(),rtWnd.Height()-rtTask.Height(),SWP_NOMOVE);	
		}
	}
	else if (m_rtCloseBtn.PtInRect(point) && m_bDownCls)
	{	
		SendMessage(hWnd,WM_CLOSE,0,0);
	}
	else if (m_rtMinBtn.PtInRect(point) && m_bDownMin)
	{	
		SendMessage(hWnd,WM_SYSCOMMAND,SC_MINIMIZE,MAKELPARAM(point.x,point.y));
	}
	else
		bResult=FALSE;

	m_bDownCls = FALSE;		//全部置为抬起状态
	m_bDownMax = FALSE;
	m_bDownMin = FALSE;	
	return bResult;		
}

/************************************************************************/
/*功能：鼠标抬起时标示系统按钮为抬起状态
/*------------------------------------------------
/*参数：同上。可以传递NULL （也可自定义用途）
/************************************************************************/
void CDrawAppearance::LButtonUP(HWND hWnd, CPoint point)
{
	m_bDownCls = FALSE;		//全部置为抬起状态
	m_bDownMax = FALSE;
	m_bDownMin = FALSE;
}
