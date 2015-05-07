#ifndef _CDRAWBUTTON_H
#define _CDRAWBUTTON_H

#include "ResDefine.h"
#include "DrawImage.h"

//定义按钮状态
enum ButtonState  {bsNormal,bsHot,bsDown};
//按钮包装类
class CDrawButton  
{
public:
    ButtonState m_State;           //按钮当前状态
    WNDPROC     m_OldProc;         //记录按钮的窗口函数
    int         m_Flag;            //是否释放关联按钮的CDrawButton对象
    static int  m_Style;           //按钮风格
    DrawImage   m_DrawObj;

public:

    CDrawButton( )
    {
        m_State   = bsNormal;
        m_OldProc = NULL;
        m_Flag    = 0;
    }

    virtual ~CDrawButton()
    {
        m_State   = bsNormal;
        m_OldProc = NULL;		
    };

    LRESULT OnPaint( HWND hWnd ) 
    {
        CWnd* pWnd = CWnd::FromHandle(hWnd);

        CPaintDC	dc(pWnd);
        CString		Text;
        CRect		RC;
        CFont		Font;
        CFont		*pOldFont;

        Font.CreateFont(12, 0, 0, 0, /*FW_NORMAL*/550, 0, 0, 0, ANSI_CHARSET, \
            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
            VARIABLE_PITCH | FF_SWISS, _T("MS Sans Serif"));
       
        pWnd->GetWindowText(Text );
        pWnd->GetClientRect( &RC );
        HRGN hRgn = CreateRectRgn( RC.left, RC.top, RC.right, RC.bottom );

        dc.SetBkMode( TRANSPARENT );
        dc.SetTextColor(RGB(40,40,40));
        pOldFont = dc.SelectObject( &Font );

        m_DrawObj.Draw(dc.m_hDC, ID_BUTTON,RC,m_State,60);
        dc.DrawText( Text, &RC, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

        pWnd->SetWindowRgn( hRgn, TRUE );
        DeleteObject( hRgn );
        dc.SelectObject( pOldFont );
        return TRUE;
    }
    // 绘制按钮
    //void  DrawButton(HWND hWnd, Gdiplus::Graphics&  graphics)
    //{
    //     CWnd* pWnd = CWnd::FromHandle(hWnd);
    //    // 获取按钮图片信息
    //    UINT iCount = m_nImageCount;
    //    int	iButtonIndex = 0;
    //    if(m_bDisabled && iCount >= 4) iButtonIndex = 3;
    //    else if(m_bPressed && iCount >= 3)iButtonIndex = 2;
    //    else if(m_bHovering && iCount >= 2)iButtonIndex = 1;
    //    else iButtonIndex = 0;

    //    // 在指定位置绘制按钮
    //    int iWidth = m_pImage->GetWidth()/iCount;
    //    int iHeight = m_pImage->GetHeight();
    //    RectF grect;
    //    grect.X=(Gdiplus::REAL)m_rcRect.left;
    //    grect.Y=(Gdiplus::REAL)m_rcRect.top;
    //    grect.Width = (Gdiplus::REAL)m_rcRect.Width();
    //    grect.Height = (Gdiplus::REAL)m_rcRect.Height();

    //    graphics.DrawImage(m_pImage, grect, (Gdiplus::REAL)iWidth*iButtonIndex,0,(Gdiplus::REAL)iWidth,(Gdiplus::REAL)iHeight, UnitPixel);

    //    StringFormat stringFormat;

    //    if (m_pIcon)
    //    {
    //        PointF ptIcon(m_ptIcon.x,m_ptIcon.y);
    //        graphics.DrawImage(m_pIcon,ptIcon);

    //        grect.X=(Gdiplus::REAL)m_ptIcon.x + m_pIcon->GetWidth() + 2;
    //        grect.Width = (Gdiplus::REAL)m_rcRect.Width() - m_pIcon->GetWidth() - 2;

    //        stringFormat.SetFormatFlags( StringFormatFlagsDirectionVertical);
    //        stringFormat.SetAlignment(StringAlignmentCenter);
    //        stringFormat.SetLineAlignment(StringAlignmentNear);
    //    }
    //    else
    //    {
    //        //stringFormat.SetFormatFlags( StringFormatFlagsDirectionVertical);
    //        stringFormat.SetAlignment(StringAlignmentCenter);
    //        stringFormat.SetLineAlignment(StringAlignmentCenter);
    //    }

    //    if (!m_strCaption.IsEmpty())
    //    {
    //        //绘制文字
    //        FontFamily fontFamily(L"宋体");
    //        Gdiplus::Font font(&fontFamily, 10, FontStyleRegular, UnitPoint);


    //        CStringW strTitle(m_strCaption);
    //        SolidBrush brush((ARGB)Color::White);
    //        if (m_bDisabled)
    //        {
    //            brush.SetColor((ARGB)Color::Gray);
    //        }

    //        graphics.DrawString(strTitle, strTitle.GetLength(), &font,grect,&stringFormat, &brush);
    //    }

    //}

    LRESULT OnLButtonDown( HWND hWnd, UINT nFlags, CPoint point )
    {
        m_State = bsDown;		
        SetCapture(hWnd);
        InvalidateRect(hWnd,NULL,TRUE);
        SetFocus(hWnd);
        return TRUE;
    }

    LRESULT OnLButtonUp( HWND hWnd, UINT nFlags, CPoint point )
    {
        if( m_State != bsNormal )
        {
            m_State = bsNormal;
            ReleaseCapture();
            InvalidateRect(hWnd,NULL,TRUE);
            SendMessage( GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), (LPARAM) (hWnd) );
        }
        return TRUE;
    }

    LRESULT LoseFocus(HWND  hWnd)
    {
        m_State = bsNormal;
        InvalidateRect(hWnd,NULL,TRUE);
        SendMessage( hWnd, WM_KILLFOCUS, (long)hWnd, 0);	
        return TRUE;
    }

    LRESULT OnMouseMove(HWND hWnd, UINT nFlags, CPoint point ) 
    {
        HRGN hRgn = CreateRectRgn( 0, 0, 0, 0 );
        GetWindowRgn( hWnd,hRgn );
        BOOL ret = PtInRegion( hRgn, point.x, point.y );
        if( ret ) 
        {

            if( m_State == bsDown) 
                return TRUE;
            else if( m_State != bsHot ) 
            {
                m_State = bsHot;
                InvalidateRect(hWnd,NULL,TRUE);
                SetCapture(hWnd);
            }
        } 
        else 
        {
            m_State = bsNormal;
            InvalidateRect(hWnd,NULL,TRUE);
            ReleaseCapture();
        }	
        DeleteObject( hRgn );
        return TRUE;
    }
   
    //窗口函数
    static LRESULT  CALLBACK ButtonWndProc(HWND hWnd, UINT uMsg, WPARAM wParam,  LPARAM lParam)
    {
        CPoint pt;
        CDrawButton *pButton=(CDrawButton*)GetWindowLong(hWnd,GWL_USERDATA);
        switch (uMsg)
        {
        case WM_LBUTTONDBLCLK:  //屏蔽双击事件
            {
                return TRUE;
            }
        case WM_PAINT:
            {
                if (pButton->m_Style>0)
                    return pButton->OnPaint( hWnd);
                else
                    return  CallWindowProc(pButton->m_OldProc, hWnd,uMsg,wParam,lParam);
            }
        case WM_LBUTTONDOWN:
            {
                pt.x = LOWORD(lParam);
                pt.y = HIWORD(lParam);
                if (pButton->m_Style>0)
                    return pButton->OnLButtonDown( hWnd, 0, pt );
                else
                    return  CallWindowProc(pButton->m_OldProc, hWnd,uMsg,wParam,lParam);
            }
        case WM_LBUTTONUP:
            {
                pt.x = LOWORD(lParam);
                pt.y = HIWORD(lParam);
                if (pButton->m_Style>0)
                    return pButton->OnLButtonUp( hWnd, 0,pt );
                else
                    return  CallWindowProc(pButton->m_OldProc, hWnd,uMsg,wParam,lParam);
            }
        case WM_MOUSEMOVE:
            {

                pt.x = LOWORD(lParam);
                pt.y = HIWORD(lParam);
                if (pButton->m_Style>0)
                    return pButton->OnMouseMove(hWnd,0, pt);
                else
                    return  CallWindowProc(pButton->m_OldProc, hWnd,uMsg,wParam,lParam);
            }

        case WM_DESTROY:
            {				
                SetWindowLong(hWnd,GWL_WNDPROC,(long)pButton->m_OldProc); //恢复原来的窗口函数

                CWnd* pWnd = CWnd::FromHandle(hWnd);  //将按钮对象与句柄分离
                if (pWnd)
                {
                    pWnd->Detach();
                }
                pButton->m_Flag = 1;
                delete pButton;
				return 0;
            }
        default :
            {
                break;
            }
        }
        return CallWindowProc(pButton->m_OldProc, hWnd, uMsg, wParam, lParam );
    }
};

int CDrawButton::m_Style = 1;

#endif//