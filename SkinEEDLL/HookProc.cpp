#include "stdafx.h"
#include "HookProc.h"
#include "include\SkinEE.h"
#include "until.h"
#include "CDrawDialog.h"
#include "CDrawButton.h"
#include "CDrawEdit.h"
#include "CDrawStatic.h"
#include "CDrawListBox.h"

CHookProc::CHookProc()
{
	SetHOOKProcMap();
}

CHookProc::~CHookProc()
{

}

CHookProc* CHookProc::getInstance()
{
	if(m_Instance == NULL){
		m_Instance = new CHookProc;
	}
	return m_Instance;
}

void CHookProc::SetHOOKProcMap()
{
    m_FuncMap[TYPE_WINDOW]= HOOKProcDialog;
    m_FuncMap[TYPE_PUSHBUTTON]= HOOKProcButton;
    m_FuncMap[TYPE_EDITBOX]= HOOKProcEdit;
    m_FuncMap[TYPE_LISTBOX]= HOOKProcListBox;
    m_FuncMap[TYPE_STATIC]= HOOKProcStatic;


    m_mapControlItems[_T("#32770")]= TYPE_WINDOW;
    m_mapControlItems[_T("Button")]= TYPE_PUSHBUTTON;
    m_mapControlItems[_T("Edit")]= TYPE_EDITBOX;
    m_mapControlItems[_T("ListBox")]= TYPE_LISTBOX;
    m_mapControlItems[_T("Static")]= TYPE_STATIC;
}

//-----------------------------------------------------------------------------------
LRESULT CHookProc::HOOKProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (HC_ACTION==nCode)
	{
		PCWPSTRUCT	wc = (PCWPSTRUCT) lParam;
		HWND  hWnd = wc->hwnd;
		if( hWnd) 
		{
			switch (wc->message)
			{
			case WM_CREATE://在这里开始子类化SubclassWindow(hWnd);
				{
					TCHAR ClassName[MAX_PATH] = _T("");
					GetClassName( hWnd, ClassName, MAX_PATH );

					int type = GetControlType((CString)ClassName);
					if (type==1||type==2||/*type==4||*/type==9||type==6)
					{
						(*m_Instance->m_FuncMap[type])(hWnd);
					}
					break;
				}
			case WM_NCDESTROY: //取消子类化UnsubclassWindow();
				{
					if(m_Instance->m_OldFunc[hWnd])  
					{ 
						SendMessage(hWnd,WM_DESTROY,NULL,NULL); 
						m_Instance->m_OldFunc.erase(hWnd);  
					}  
					break;
				}
			default:
				break;
			}
		}
	}
	return CallNextHookEx( NULL, nCode, wParam, lParam );
}


LRESULT  CHookProc::HOOKProcDialog(HWND hWnd)
{
	CWnd	*pWnd = CWnd::FromHandle( hWnd );

	if (pWnd->GetStyle() & /*DS_SYSMODAL*/DS_ABSALIGN)			//过滤掉消息对话框
	{
		return  1;
	}

	CDrawDialog *pDlg=new CDrawDialog(hWnd);
	SetWindowLong(hWnd,GWL_USERDATA,(long)pDlg);
	pDlg->m_OldProc = (WNDPROC) SetWindowLong( hWnd, GWL_WNDPROC, (LONG)CDrawDialog::DialogWndProc );
	m_Instance->m_OldFunc[hWnd] = pDlg->m_OldProc;

	return 0;
}

LRESULT  CHookProc::HOOKProcButton(HWND hWnd)
{
    CWnd	*pWnd = CWnd::FromHandle( hWnd );

	long c=pWnd->GetStyle();
    if( pWnd->GetStyle() & BS_RADIOBUTTON ||pWnd->GetStyle() & BS_CHECKBOX /*||(pWnd->GetStyle() & BS_DEFPUSHBUTTON)!=1*/)
    {
        return 1;
    }
	if((pWnd->GetStyle() & BS_AUTORADIOBUTTON)==9 )//单选按钮
	{
		return 1;
	}
	
    CDrawButton *pButton = new CDrawButton;;
    SetWindowLong(hWnd,GWL_USERDATA,(long)pButton);
    pButton->m_OldProc =  (WNDPROC) SetWindowLong( hWnd, GWL_WNDPROC, (LONG) CDrawButton::ButtonWndProc);
	m_Instance->m_OldFunc[hWnd] = pButton->m_OldProc;

	return 0;
}

LRESULT  CHookProc::HOOKProcEdit(HWND hWnd)
{
    CWnd	*pWnd = CWnd::FromHandle( hWnd );
    pWnd->ModifyStyleEx(WS_EX_CLIENTEDGE,0,0);	//取消编辑框的边框

    CDrawEdit *pEdit=new CDrawEdit();
    SetWindowLong(hWnd,GWL_USERDATA,(long)pEdit);
    pEdit->m_OldProc =  (WNDPROC) SetWindowLong( hWnd, GWL_WNDPROC, (LONG) CDrawEdit::EditWndProc);
    m_Instance->m_OldFunc[hWnd] =pEdit->m_OldProc;
	
	return 0;
}

LRESULT  CHookProc::HOOKProcListBox(HWND hWnd)
{
	int nStyle=GetWindowLong(hWnd,GWL_STYLE);  
	if(!(nStyle & LBS_OWNERDRAWFIXED))  
	{  
        CDrawListBox *pListBox=new CDrawListBox();
        SetWindowLong(hWnd,GWL_USERDATA,(long)pListBox);
		//SetWindowLong(hWnd,GWL_STYLE,GetWindowLong(hWnd,GWL_STYLE)|LBS_OWNERDRAWFIXED|LBS_HASSTRINGS);  
		pListBox->m_OldProc=(WNDPROC)SetWindowLong(hWnd,GWL_WNDPROC,(long)CDrawListBox::ListBoxWndProc);  
	    m_Instance->m_OldFunc[hWnd] = pListBox->m_OldProc;
    }  
	return 0;
}

LRESULT  CHookProc::HOOKProcStatic(HWND hWnd)
{
    int nStyle=GetWindowLong(hWnd,GWL_STYLE);  
    if(!(nStyle & SS_OWNERDRAW))  
    {  
		CDrawStatic *pStatic=new CDrawStatic(); 
		SetWindowLong(hWnd,GWL_USERDATA,(long)pStatic);
        pStatic->m_OldProc=(WNDPROC)SetWindowLong(hWnd,GWL_WNDPROC,(long)CDrawStatic::StaticWndProc);
		m_Instance->m_OldFunc[hWnd] =pStatic->m_OldProc;
    }  
    return 0;
}

//-------------------------------已废弃------------------------------------

LRESULT CHookProc::ListBoxWndProc(HWND hWnd, UINT uMsg, WPARAM wParam,  LPARAM lParam)
{
	return 0;
}

//----------------------------WH_CBT--------------------------------
LRESULT CHookProc::HOOKProc_CBT(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode==HCBT_CREATEWND)  
	{  
		HWND hWin=(HWND)wParam;  
		CBT_CREATEWND* s=(CBT_CREATEWND*)lParam;  
		TCHAR szClass[MAX_PATH]={0};  

		GetClassName(hWin,szClass,MAX_PATH); 
		int type=GetControlType((CString)szClass);
		if (type==1)
		{
			(*m_Instance->m_FuncMap[type])(hWin/*nCode, wParam, lParam*/);
		}

		return 0;
	}
	else if(nCode==HCBT_DESTROYWND)  
	{  
		HWND hWin=(HWND)wParam;  
		if(m_Instance->m_OldFunc[hWin])  
		{  
			SetWindowLong(hWin,GWL_WNDPROC,(long)m_Instance->m_OldFunc[hWin]);  
			m_Instance->m_OldFunc.erase(hWin);  
		}  
	}  
	return CallNextHookEx(NULL,nCode,wParam,lParam); 
}

LRESULT  CHookProc::HOOKProcDialog_CBT(HWND hWnd)
{
	CWnd *pWnd = CWnd::FromHandle( hWnd );

	SetWindowLong(hWnd,GWL_STYLE,GetWindowLong(hWnd,GWL_STYLE)&~WS_SYSMENU);  
	m_Instance->m_OldFunc[hWnd]=(WNDPROC)SetWindowLong(hWnd,GWL_WNDPROC,(long)CDrawDialog::DialogWndProc);
	return 0;
}

LRESULT CHookProc::DialogWndProc_CBT(HWND hwnd, UINT uMsg, WPARAM wParam,  LPARAM lParam)
{
	if(uMsg==WM_MEASUREITEM)  
	{  
		LPMEASUREITEMSTRUCT ms=(LPMEASUREITEMSTRUCT)lParam;  
		HWND hWin=GetDlgItem(hwnd,ms->CtlID);  
		SendMessage(hWin,WM_MYMEASUREITEM,wParam,lParam);  
		return 0;  
	}  
	else if(uMsg==WM_DRAWITEM)  
	{  
		LPDRAWITEMSTRUCT ds=(LPDRAWITEMSTRUCT)lParam;  
		HWND h=GetDlgItem(hwnd,ds->CtlID);  
		::SendMessage(ds->hwndItem,WM_MYDRAWITEM,wParam,lParam);  
		return 0;  
	}  
	else if(uMsg==WM_CTLCOLOREDIT)  
	{  
		HBRUSH b=(HBRUSH)::SendMessage(HWND(lParam),WM_MYCTLCOLOR,wParam,lParam);  
		return (long)b;  
	}else if(uMsg==WM_CTLCOLORDLG)  
	{  
		CDC dc;  
		dc.Attach((HDC)wParam);  
		static CBrush b;  
		static bool bFont=false;  
		b.DeleteObject();  
		b.CreateSolidBrush(RGB(253,255,240));  
		dc.Detach();  
		return (long)(HBRUSH)b;  
	}  
	else if(uMsg==WM_NCPAINT)  
	{  
		HDC hdc=GetWindowDC(hwnd);  
		CDC *dc=CDC::FromHandle(hdc);  
		CRect r1,r2,rLeft,rTop,rRight,rBottom;  
		GetWindowRect(hwnd,r1);  

		GetClientRect(hwnd,&r2);  
		CWnd::FromHandle(hwnd)->ClientToScreen(&r2);  
		rLeft.left=0;  
		rLeft.top=0;  
		rLeft.right=r2.left-r1.left;  
		rLeft.bottom=r1.Height();  
		rTop.left=0;  
		rTop.top=0;  
		rTop.right=r1.Width();  
		rTop.bottom=r2.top-r1.top;  
		rRight.left=r2.left-r1.left+r2.Width();  
		rRight.top=0;  
		rRight.right=r1.Width();  
		rRight.bottom=r1.Height();  
		rBottom.left=0;  
		rBottom.top=r2.top-r1.top+r2.Height();  
		rBottom.right=r1.Width();  
		rBottom.bottom=r1.Height();  
		dc->FillSolidRect(&rLeft,RGB(113,113,113));  
		dc->FillSolidRect(&rTop,RGB(113,113,113));  
		dc->FillSolidRect(&rRight,RGB(113,113,113));  
		dc->FillSolidRect(&rBottom,RGB(113,113,113));  
		CRect rClose;  
		rClose.left=r1.Width()-30;  
		rClose.right=r1.Width()-10;  
		rClose.top=3;  
		rClose.bottom=28;  
		dc->Draw3dRect(&rClose,RGB(250,250,250),RGB(250,250,250));  
		dc->SetBkMode(TRANSPARENT);  
		dc->SetTextColor(RGB(250,250,250));  
		dc->DrawText(_T("X"),&rClose,DT_CENTER|DT_VCENTER|DT_SINGLELINE);  
		HWND hPar=GetParent(hwnd);  
		if(hPar==GetDesktopWindow() || IsWindowEnabled(hPar) || hwnd==AfxGetMainWnd()->m_hWnd)  
		{  
			CRect rMini;  
			rMini.left=rClose.left-25;  
			rMini.right=rClose.left-5;  
			rMini.top=3;  
			rMini.bottom=28;  
			dc->Draw3dRect(&rMini,RGB(250,250,250),RGB(250,250,250));  
			dc->DrawText(_T("_"),&rMini,DT_CENTER|DT_VCENTER|DT_SINGLELINE);  
		}  
		TCHAR szTitle[_MAX_PATH]={0};  
		GetWindowText(hwnd,szTitle,_MAX_PATH);  
		dc->TextOut(5,10,szTitle);  
		ReleaseDC(hwnd,hdc);  
		return 0;  
	}else if(uMsg==WM_NCLBUTTONDOWN)  
	{  
		CPoint p;  
		GetCursorPos(&p);  
		CRect r,r1,r2;  
		GetWindowRect(hwnd,&r);  
		CRect rClose;  
		rClose.left=r.Width()-30;  
		rClose.right=r.Width()-10;  
		rClose.top=3;  
		rClose.bottom=28;  
		r1=rClose;  
		CRect rMini;  
		rMini.left=rClose.left-25;  
		rMini.right=rClose.left-5;  
		rMini.top=3;  
		rMini.bottom=28;  
		r2=rMini;  
		r1.OffsetRect(r.TopLeft());  
		r2.OffsetRect(r.TopLeft());  
		if(PtInRect(&r1,p))  
		{  
			PostMessage(hwnd,WM_SYSCOMMAND,SC_CLOSE,MAKELPARAM(p.x,p.y));  
			return 0;  
		}  
		HWND hPar=GetParent(hwnd);  
		if(hPar==GetDesktopWindow() || IsWindowEnabled(hPar) || hwnd==AfxGetMainWnd()->m_hWnd)  
		{  
			if(PtInRect(&r2,p))  
			{  
				PostMessage(hwnd,WM_SYSCOMMAND,SC_MINIMIZE,MAKELPARAM(p.x,p.y));  
				return 0;  
			}  
		}  
	}else if(uMsg==WM_NCACTIVATE)  
	{  
		SendMessage(hwnd,WM_NCPAINT,0,0);  
		return 1;  
	}else if(uMsg==WM_SETTEXT)  
	{  
		CallWindowProc(m_Instance->m_OldFunc[hwnd],hwnd,uMsg,wParam,lParam);  
		SendMessage(hwnd,WM_NCPAINT,0,0);  
		return 1;  
	}  
	return CallWindowProc(m_Instance->m_OldFunc[hwnd],hwnd,uMsg,wParam,lParam);  
}