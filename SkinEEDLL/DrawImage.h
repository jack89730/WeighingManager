#ifndef _SKINEE_IMAGE_H
#define _SKINEE_IMAGE_H 

#include "SkinEEManager.h"

class DrawImage
{
public:
	 DrawImage()
	 {

	 }
	
	~DrawImage()
	{

	}
	
    //ImageList_V
	void Draw(HDC hDC,int nResImage,CRect rect,int nSub,int subWidth)
	{   
		Graphics graphics(hDC);
		Image *pImage = CSkinManager::GetInstance()->GetResItem(nResImage);
        if (pImage==NULL)
            return;

		int srcwidth=subWidth;
		int srcheight=pImage->GetHeight();
		int srcx=srcwidth*nSub;
		int srcy=0;

		RectF grect;
		grect.X=(Gdiplus::REAL)rect.left;
		grect.Y=(Gdiplus::REAL)rect.top;
		grect.Width = (Gdiplus::REAL)rect.Width();
		grect.Height = (Gdiplus::REAL)rect.Height();
		graphics.DrawImage(pImage, grect, srcx,srcy,srcwidth,srcheight, UnitPixel);
	}

	void Draw(HDC hDC,int nResImage,CRect rect)
	{
		RectF grect;
		grect.X=(Gdiplus::REAL)rect.left;
		grect.Y=(Gdiplus::REAL)rect.top;
		grect.Width = (Gdiplus::REAL)rect.Width();
		grect.Height = (Gdiplus::REAL)rect.Height();

		Graphics graphics( hDC );
		Image *pImage = CSkinManager::GetInstance()->GetResItem(nResImage);
		graphics.DrawImage(pImage,grect ,0,0,pImage->GetWidth(),pImage->GetHeight(),UnitPixel);
	}

    void Draw(CDC *pWinDC,int nResImage,CRect rect,int srcx,int srcy,int srcwidth,int srcheight)
    {    
        Graphics graphics( pWinDC->m_hDC );
        Image *pImage = CSkinManager::GetInstance()->GetResItem(nResImage);
      
        int a=pImage->GetWidth();
        RectF grect;
        grect.X=(Gdiplus::REAL)rect.left;
        grect.Y=(Gdiplus::REAL)rect.top;
        grect.Width = (Gdiplus::REAL)rect.Width();
        grect.Height = (Gdiplus::REAL)rect.Height();
        graphics.DrawImage(pImage, grect, srcx,srcy,srcwidth,srcheight, UnitPixel);
    }

	BOOL DrawBackground(HDC hDC, CRect rect, COLORREF clrBrush, UINT nResImage
		/*BOOL bCenter,BOOL bTransparent,COLORREF clrTransparent*/)	
	{
		BOOL bSet=FALSE;									//返回值		

		Graphics graphics(hDC);
		Image *pImage = CSkinManager::GetInstance()->GetResItem(nResImage);

		if (clrBrush!=NULL)									//设置背景画刷
		{	
			SolidBrush brush(clrBrush);	
			graphics.FillRectangle(&brush,rect.left,rect.top,rect.Width(),rect.Height());
		}	
		if (pImage!=NULL)								    //设置背景位图
		{
			RectF grect;
			grect.X=(Gdiplus::REAL)rect.left;
			grect.Y=(Gdiplus::REAL)rect.top;
			grect.Width = (Gdiplus::REAL)rect.Width();
			grect.Height = (Gdiplus::REAL)rect.Height();

			graphics.DrawImage(pImage, grect);
		}					
		return bSet;
	}

	void  DrawString(HDC hDC,TCHAR* text,PointF pointf ,Color color,int size,int style=FontStyleRegular)
	{
		Graphics graphics(hDC);

		SolidBrush brush(color);//创建一个笔刷 第一项是透明度 后三项是RGB值Color(100,0,0,255)
		FontFamily fontFamily(L"Arial");//选择一种字体
		Font font(&fontFamily,size,style,UnitPixel);
		StringFormat stringformat;  
		stringformat.SetAlignment(StringAlignmentCenter);  

		graphics.DrawString(text,-1,&font,pointf, &stringformat,&brush);
	}

    void  DrawString(CDC *pWinDC,TCHAR* string,Font* font1,PointF pointF,Color color)
    {
        Graphics graphics( pWinDC->m_hDC );

        //---------------------------------------   
        // fontFamily第一个参数是用来定义   
        // 宋体、楷体、仿宋、黑体、隶书以及幼圆等多种字样   
        // 特别说明:Gdi+不支持OpenType字体，吃了一次亏了   
        //---------------------------------------   
        FontFamily fontFamily(L"幼圆");   

        //---------------------------------------   
        // font四个参数，第二个：字体高度，单位由第四个决定   
        // 第三个：字体风格:   
        //  FontStyleRegular(正常)、   
        //  FontStyleBold(加粗)、   
        //  FontStyleItalic(斜体)、   
        //  FontStyleBoldItalic (粗斜体)、   
        //  FontStyleUnderline(下划线)   
        //  FontStyleStrikeout(删除线)   
        // 第四个:UnitDisplay(1/75英寸)、   
        //  UnitPixel(像素)、   
        //  UnitPoint(点)、一点约折合为1/72英寸   
        //  UnitInch(英寸)、   
        //  UnitDocument(1/300英寸)、   
        //  UnitMillimeter(毫米)   
        //---------------------------------------   
        Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel); 

        //---------------------------------------   
        // 这是为了和以前的GDI字体   
        // font两个参数:   
        //  HDC， LOGFONT   
        //---------------------------------------   
        LOGFONT lf;  
        ::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(lf), &lf);  
        memcpy(lf.lfFaceName, "Arial", 5) ;  
        lf.lfHeight = 13 ;  

        Gdiplus::Font Gdi_font(pWinDC->GetSafeHdc(), &lf);  

        //-----------------------------------------   
        //stringformat主要用以下的函数来控制文本的显示方式   
        // SetAlignment   
        //-----------------------------------------   
        StringFormat stringformat;  

        //-----------------------------------------   
        //  StringAlignmentNear(左对齐或右对齐，取决于书写方向是从左到右还是从右到左)、   
        //  StringAlignmentCenter(水平居中)   
        //  StringAlignmentFar(两端对齐)。   
        //  默认都不影响垂直   
        //-----------------------------------------   
        stringformat.SetAlignment(StringAlignmentCenter);  

        //-----------------------------------------   
        // 和SetAlignment对应，为竖直的设置   
        //-----------------------------------------   
        stringformat.SetLineAlignment(StringAlignmentCenter);  

        //------------------------------------------   
        // SolidBrush   
        // HatchBrush(阴影画刷)   
        // TextureBrush(纹理画刷):使用图像来填充形状的内部。无法继承此类   
        // LinearGradientBrush(渐变画刷)   
        // 注意，用的是Color初始化，不要用RGB初始化，不会报错，但绘不出来！   
        //------------------------------------------   
        SolidBrush brush(Color(255, 255, 0, 0));  

        //------------------------------------------   
        // SetTextRenderingHint控制文本输出的质量   
        //------------------------------------------   
        graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);  


        //------------------------------------------   
        // 一种是point ,一种是rect控制输出   
        //------------------------------------------   
        CRect rcClient ;  
        //GetClientRect(rcClient);  

        graphics.DrawString(TEXT("1234"), -1, &font,  
            RectF(0, 0, rcClient.Width(), rcClient.Height()), &stringformat, &brush); 
    }

	void GetImageRect(int nResImage,CRect& rect)
	{
		Image *pImage = CSkinManager::GetInstance()->GetResItem(nResImage);
	    rect.SetRect(0,0,pImage->GetWidth(),pImage->GetHeight());
	}

    void GetImageColor(int nResImage,Color* color)
    {
        Bitmap *pImage = (Bitmap*)CSkinManager::GetInstance()->GetResItem(nResImage);
        //ColorPalette Palette;
        pImage->GetPixel(5,5,color);
    }


	//---------------------------------------------GDI------------------------------------------
	 void Draw(CDC *pWinDC,CString strImage,CRect rect)
	 {
		 CBitmap bmpObj,*OldObj;
		 BITMAPINFO bitinfo;
		 CDC memDC;
		 memDC.CreateCompatibleDC(NULL);

		 HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL,
			 strImage,
			 IMAGE_BITMAP,
				0,
				0,
				LR_LOADFROMFILE);

		 bmpObj.Attach(hBitmap);
		 bmpObj.GetObject(sizeof(bitinfo),&bitinfo);//获取位图大小
		 OldObj = memDC.SelectObject(&bmpObj);
		 int x = bitinfo.bmiHeader.biWidth;
		 int y = bitinfo.bmiHeader.biHeight;
		 pWinDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC,0,0,x,y,SRCCOPY);
         int a=rect.Width();
		 memDC.SelectObject(OldObj);
		 bmpObj.DeleteObject();
		 memDC.DeleteDC();	
	 }

     void GetImageRect(CString strImage,CRect& rect)
     {
         CBitmap bmpObj;
         BITMAPINFO bitinfo;

         rect.SetRect(0,0,0,0);
         HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL,
             strImage,
             IMAGE_BITMAP,
             0,
             0,
             LR_LOADFROMFILE);

         bmpObj.Attach(hBitmap);
         bmpObj.GetObject(sizeof(bitinfo),&bitinfo);//获取位图大小
         rect.right = bitinfo.bmiHeader.biWidth;
         rect.bottom = bitinfo.bmiHeader.biHeight;
         bmpObj.DeleteObject();
     }

	 void  DrawString(CDC *pWinDC,TCHAR* text,CRect rect ,Color color=(200,255,255,0),Font* font1=NULL)
	 {
		 pWinDC->SetBkMode(TRANSPARENT);
		 pWinDC->SetTextColor(RGB(255,255,0));
		 pWinDC->SetTextAlign(TA_CENTER);

		 //pWinDC->SelectObject(&m_CaptionFont);
		 pWinDC->TextOut(rect.Width(), rect.Height() ,text);
	
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
     BOOL DrawBackground(CDC *pWinDC, CRect *pDesRect, COLORREF clrBrush, UINT unIDBitmap,
         BOOL bCenter,BOOL bTransparent,COLORREF clrTransparent)	
     {
         BOOL bSet=FALSE;									//返回值		
         if (pWinDC==NULL && !IsWindow(pWinDC->GetWindow()->m_hWnd))
         {	AfxMessageBox(L"无效的DC,或窗口不存在！",MB_ICONASTERISK);
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
                 AfxMessageBox(L"Load bitmap failure!\r\rThe bitmap exist?",MB_ICONERROR|MB_OK);
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
};

#endif//