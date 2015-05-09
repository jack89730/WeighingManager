#ifndef _CDRAWEDIT_H
#define _CDRAWEDIT_H
//定义编辑框类
class CDrawEdit
{
public:
	WNDPROC     m_OldProc;         //记录编辑框的窗口函数
	int         m_Flag;
	CBrush      m_brush;
	CRect       m_rect;
	static int  m_Style; 
public:
	CDrawEdit()
	{
		m_OldProc = NULL;
		m_Flag = 0;
	}

	void OnErasebkgnd(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		CDC* dc = CDC::FromHandle(HDC(wParam));	//获取画布对象
		/*CPaintDC dc(CWnd::FromHandle(hWnd)); */
		dc->FrameRect(m_rect, &m_brush);		    //绘制边框
	}

	void OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		GetClientRect(hWnd, m_rect);			    //获取客户区域
		m_rect.InflateRect(1, 1, 1, 1);		    //将客户区域增大一个像素
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(RGB(0, 64, 255));	//创建画刷
	}

	//编辑框窗口函数
	static LRESULT CALLBACK EditWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		CPoint pt;
		CDrawEdit *pEdit = (CDrawEdit*)GetWindowLong(hWnd, GWL_USERDATA);
		switch (uMsg)
		{
		case WM_CREATE:
		{
						  TRACE("Edit发送WM_CREATE\n");
						  pEdit->OnCreate(hWnd, wParam, lParam);
						  break;
		}
		case WM_ERASEBKGND:
		{
			TRACE("Edit发送WM_ERASEBKGND\n");
			pEdit->OnErasebkgnd(hWnd, wParam, lParam);
							  break;
		}
		case WM_PAINT:
		{
							  TRACE("Edit发送WM_ERASEBKGND\n");
							  pEdit->OnErasebkgnd(hWnd, wParam, lParam);
							  break;
		}

		case WM_DESTROY:
		{
						   SetWindowLong(hWnd, GWL_WNDPROC, (long)pEdit->m_OldProc); //恢复原来的窗口函数
						   CWnd* pWnd = ::CWnd::FromHandle(hWnd);			//将按钮对象与句柄分离
						   if (pWnd)
						   {
							   pWnd->Detach();
						   }
						   pEdit->m_Flag = 1;
						   delete pEdit;
						   return 0;
		}

		default:
		{
				   break;
		}
		}
		return CallWindowProc(pEdit->m_OldProc, hWnd, uMsg, wParam, lParam);
	}
};
int CDrawEdit::m_Style = 1;

#endif//