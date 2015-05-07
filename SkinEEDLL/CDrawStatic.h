
class CDrawStatic
{
public:
	WNDPROC     m_OldProc;         //记录的窗口函数
	TCHAR       m_szText[MAX_PATH];
	COLORREF    m_color;
    CRect       m_rect;

public:
	CDrawStatic()
	{
		m_OldProc = NULL;
		ZeroMemory(m_szText,MAX_PATH) ;
	}
	void SetText(TCHAR* text)
	{
		_tcscpy_s(m_szText, MAX_PATH,text);
	};

	void OnPaint(HWND hWnd,WPARAM wParam, LPARAM lParam )
	{
        CPaintDC dc(CWnd::FromHandle(hWnd)); 
		dc.SetBkMode(TRANSPARENT);
        CFont		Font;
        CFont		*pOldFont;
        Font.CreateFont(12,0,0,0,/*FW_NORMAL*/550,0,0,0,ANSI_CHARSET,OUT_TT_PRECIS,
            CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,VARIABLE_PITCH|FF_SWISS,
            _T("MS Sans Serif"));
        pOldFont = dc.SelectObject( &Font );
		//dc.DrawText(m_szText,&rect,DT_EDITCONTROL|DT_WORDBREAK|DT_CALCRECT);   
		dc.DrawText(m_szText,&m_rect,DT_EDITCONTROL|DT_WORDBREAK); 
        dc.SelectObject( pOldFont );
		/*DrawImage draw;
		draw.DrawString(&dc,m_szText,PointF(rect.left+20,rect.top-2),Color(255,0,0,0),12);*/
	}

	//窗口函数
	static LRESULT CALLBACK StaticWndProc(HWND hWnd,UINT uMsg,WPARAM wParam, 
		LPARAM lParam )
	{
		CDrawStatic *pStatic=(CDrawStatic*)GetWindowLong(hWnd,GWL_USERDATA);
		switch (uMsg)
		{
		case WM_PAINT:
			{
				TRACE("static发送WM_PAINT\n");
                pStatic->OnPaint(hWnd,wParam,lParam);
				break;
			}
        case WM_SETTEXT:
            {
                TRACE("static发送WM_SETTEXT\n");
                pStatic->SetText(_T(""));
                InvalidateRect(hWnd,&pStatic->m_rect,TRUE);
                pStatic->SetText((TCHAR*)lParam);
                InvalidateRect(hWnd,&pStatic->m_rect,TRUE);
                break;
            }
         case WM_CREATE:
             {
                 TRACE("static发送WM_SETTEXT\n");
                 GetClientRect(hWnd,&pStatic->m_rect);
                 GetWindowText(hWnd,pStatic->m_szText,MAX_PATH);
                 break;
             }

		 case WM_ERASEBKGND:
			 {
			   TRACE("static发送WM_ERASEBKGND\n");
			   break;
			 }
	
		case WM_DESTROY:
			{				
				SetWindowLong(hWnd,GWL_WNDPROC,(long)pStatic->m_OldProc); //恢复原来的窗口函数
				CWnd* pWnd = ::CWnd::FromHandle(hWnd);			//将按钮对象与句柄分离
				if (pWnd)
				{
					pWnd->Detach();
				}
                delete pStatic;
				return 0;
			}
		default :
			{
				break;
			}
		}
		return CallWindowProc(pStatic->m_OldProc, hWnd, uMsg, wParam, lParam );
	}
};