class CDrawListBox 
{
protected:
   
public:
    WNDPROC     m_OldProc;         //记录窗口函数
    int         m_nMaxWidth;
    int         m_Flag; 
 
public:
    CDrawListBox::CDrawListBox()
    {
        m_nMaxWidth=0;
        m_Flag    = 0;
    }

    virtual CDrawListBox::~CDrawListBox()
    {
        m_OldProc=NULL;
    }
    /*virtual */void DrawItem(HWND hWnd,  WPARAM wParam,  LPARAM lParam)
    {
        LPDRAWITEMSTRUCT lpDrawItemStruct=(LPDRAWITEMSTRUCT)lParam;
        CWnd *pWmd =CWnd::FromHandle(hWnd);
        CListBox *p=(CListBox*)pWmd;
        if(p->GetCount()==0)
            return;
        CDC dc;
        //int nIndex;
        dc.Attach(lpDrawItemStruct->hDC);
        COLORREF bk=dc.GetBkColor();
        COLORREF fg=dc.GetTextColor();
        if(/*lpDrawItemStruct->itemAction & ODA_FOCUS && */lpDrawItemStruct->itemState & ODS_FOCUS   || 
            /*lpDrawItemStruct->itemAction & ODA_SELECT && */(lpDrawItemStruct->itemState & ODS_SELECTED))
        {
            bk=RGB(200,200,200);
            if(lpDrawItemStruct->itemData)
                fg=RGB(0,128,0);
            else
                fg=RGB(0,0,255);
        }
        else
        {
            if(lpDrawItemStruct->itemData)
            {
                bk=RGB(210,255,210);
                fg=RGB(0,128,0);
            }
            else
            {
                bk=RGB(215,215,255);
                fg=RGB(0,0,255);
            }
        }
        CRect rectItem(lpDrawItemStruct->rcItem);
        CBrush brush(bk);
        dc.FillRect(&rectItem,&brush);
        CString strText;
        p->GetText(lpDrawItemStruct->itemID,strText);
        dc.SetBkMode(TRANSPARENT);
        dc.SetTextColor(fg);
        dc.DrawText(strText,rectItem,DT_LEFT|DT_VCENTER);
        rectItem.DeflateRect(0,rectItem.Height()/2,0,0);
        dc.SetTextColor(RGB(0,0,0));
        dc.DrawText(strText,rectItem,DT_LEFT | DT_VCENTER);
        dc.Detach();
    }
   virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct )
    {
        lpMeasureItemStruct->itemHeight=lpMeasureItemStruct->itemHeight*2;
    }
   //窗口函数
   static LRESULT CALLBACK ListBoxWndProc(HWND hWnd, UINT uMsg, WPARAM wParam,  LPARAM lParam)
   {
       CDrawListBox *pListBox=(CDrawListBox*)GetWindowLong(hWnd,GWL_USERDATA);
      // PAINTSTRUCT ps;
       switch (uMsg)
       {  
       case WM_PAINT:
           {
               //pListBox->DrawItem(hWnd, wParam, lParam);
               break;
           }
       case WM_DRAWITEM:
           {
               pListBox->DrawItem(hWnd, wParam, lParam);

           }

       case WM_CTLCOLORDLG:
           {
               TRACE(" 窗口发送WM_CTLCOLORDLG\n");
               break;
           }

       case WM_CREATE:
           {
               TRACE(" 窗口发送WM_CREATE\n");
               
               break;
           }

       case WM_DESTROY:
           {
               //TRACE("%S 窗口发送WM_DESTROY\n",pDlg->m_Text);
               SetWindowLong(hWnd,GWL_WNDPROC,(long)pListBox->m_OldProc);
               CWnd* pWnd = ::CWnd::FromHandle(hWnd);			//将对象与句柄分离
               if (pWnd)
               {
                   pWnd->Detach();
               }
               pListBox->m_Flag = 1;
               delete pListBox;
			   return 0;
           }

       default :
           break;	
       }
       return CallWindowProc(pListBox->m_OldProc,hWnd,uMsg,wParam,lParam);
   }
};