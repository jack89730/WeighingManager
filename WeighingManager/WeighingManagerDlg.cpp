
// WeighingManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WeighingManager.h"
#include "WeighingManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWeighingManagerDlg 对话框



CWeighingManagerDlg::CWeighingManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWeighingManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_BKColor = RGB(165, 201, 235);
	m_BKBrush.CreateSolidBrush(RGB(160, 200, 235));
}

void CWeighingManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWeighingManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCPAINT()
	ON_WM_CTLCOLOR()
	//ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CWeighingManagerDlg 消息处理程序

BOOL CWeighingManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	theApp.serialPort1.InitPort(this);

	InitImageList();
	InitToolBar();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWeighingManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWeighingManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWeighingManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWeighingManagerDlg::InitImageList()
{
	m_ImageListToolbar.Create(48, 48, ILC_COLOR24 | ILC_MASK, 1, 1);
	m_ImageListToolbar.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageListToolbar.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_ImageListToolbar.Add(AfxGetApp()->LoadIcon(IDI_ICON5));
	m_ImageListToolbar.Add(AfxGetApp()->LoadIcon(IDI_ICON7));

	m_ImageListHotToolbar.Create(48, 48, ILC_COLOR24 | ILC_MASK, 1, 1);
	m_ImageListHotToolbar.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageListHotToolbar.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_ImageListHotToolbar.Add(AfxGetApp()->LoadIcon(IDI_ICON5));
	m_ImageListHotToolbar.Add(AfxGetApp()->LoadIcon(IDI_ICON7));

}

BOOL CWeighingManagerDlg::InitToolBar()
{
	UINT array[8];
	array[0] = IDC_TOOLBAR_BUTTON1;//录音查询
	array[1] = IDC_TOOLBAR_BUTTON2;//系统配置参数
	array[2] = IDC_TOOLBAR_BUTTON3;//通道参数
	array[3] = IDC_TOOLBAR_BUTTON4;//用户管理

	m_ToolBar.Create(this);
	m_ToolBar.SetButtons(array, 4);

	m_ToolBar.SetButtonText(0, _T("自动识别"));
	m_ToolBar.SetButtonText(1, _T("手动识别"));
	m_ToolBar.SetButtonText(2, _T("生成报表"));
	m_ToolBar.SetButtonText(3, _T("系统设置"));

	m_ToolBar.GetToolBarCtrl().SetImageList(&m_ImageListToolbar);//关联图片到按钮上
	m_ToolBar.GetToolBarCtrl().SetHotImageList(&m_ImageListToolbar);
	m_ToolBar.GetToolBarCtrl().SetDisabledImageList(&m_ImageListToolbar);
	m_ToolBar.GetToolBarCtrl().SetButtonWidth(60, 200);//设置按钮的宽度
	m_ToolBar.SetSizes(CSize(68, 68), CSize(48, 48));//设置按钮大小
	m_ToolBar.EnableToolTips(TRUE);//激活提示信息

	//m_ToolBar.GetToolBarCtrl().SetState(IDC_TOOLBAR_BUTTON2, TBSTATE_INDETERMINATE);
	//m_ToolBar.GetToolBarCtrl().SetState(IDC_TOOLBAR_BUTTON2, TBSTATE_ENABLED);
	//m_ToolBar.GetToolBarCtrl().SetState(IDC_TOOLBAR_BUTTON2, TBSTATE_HIDDEN);
	
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, CWnd::reposDefault, NULL, NULL, true);

	m_ToolBar.GetToolBarCtrl().SetWindowPos(&CWnd::wndBottom, 10, 10, 100, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);


	//第二种方法加载位图
	{
		////使工具栏图标显示256色
		//HBITMAP hbm = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),  
		//	MAKEINTRESOURCE(IDR_TOOLBAR1), //加载IDB_TOOLBAR 
		//	IMAGE_BITMAP, //按位图格式
		//	0,0, // cx,cy  
		//	LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS ); 
		//CBitmap bm;  
		//bm.Attach(hbm); 
		//m_ilToolBar.Create(48,48,ILC_COLOR8, 4, 4); 
		//m_ilToolBar.Add(&bm,(CBitmap*)NULL); 
	}

	return true;
}
LRESULT CWeighingManagerDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//return CDialog::DefWindowProc(message, wParam, lParam);
	LRESULT lrst = CDialog::DefWindowProc(message, wParam, lParam);
	if (message == WM_NCPAINT || message == WM_MOVE || message == WM_NCACTIVATE || message == WM_PAINT/*||message == WM_NOTIFY*/)
	{
		CDC* pWinDC = GetWindowDC();
		if (pWinDC)
		{
			m_drawApe.DrawDlg(pWinDC, m_BKColor);
			m_drawApe.DrawTitleBar(pWinDC, m_BKColor);
			m_drawApe.DrawIcon(pWinDC, IDI_ICON1, 2);
			//m_drawApe.DrawDlg(pWinDC,IDB_BITMAP_BG,IDB_BITMAP_BG,IDB_BITMAP_BG,IDB_BITMAP_BG);

			//DrawTitleBar(pWinDC);
			//DrawDlg(m_BKColor);
			//DrawDlg(IDB_BITMAP_BG,IDB_BITMAP_BG,IDB_BITMAP_BG,IDB_BITMAP_BG);

			TCHAR szWndTitle[MAX_PATH];
			ZeroMemory(szWndTitle, sizeof(szWndTitle));
			GetWindowText(szWndTitle, sizeof(TCHAR)*MAX_PATH);
			//SendMessage(hWnd, WM_GETTEXT, (WPARAM)MAX_PATH, (LPARAM)szWndTitle); 
			m_drawApe.SetTitleColor(RGB(0, 0, 0));
			m_drawApe.DrawTitle(pWinDC, szWndTitle, 20);
			//m_drawApe.DrawSystemBtn1(pWinDC, IDB_NORMAL);
		}
		ReleaseDC(pWinDC);
	}
	return lrst;
}

void CWeighingManagerDlg::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnNcPaint()
}

//void CWeighingManagerDlg::DrawTitleBar(CDC *pDC)
//{
//	CDC memDC;
//	CBitmap bmp;
//	CRect rect, rtTitle;
//	CPoint point;
//	GetClientRect(&rect);
//
//	//获取标题栏大小
//	rtTitle.left = GetSystemMetrics(SM_CXFRAME);
//	rtTitle.top = GetSystemMetrics(SM_CYFRAME);
//	rtTitle.right = rect.right - rect.left - GetSystemMetrics(SM_CXFRAME);
//	rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);
//
//	point.x = rect.right + GetSystemMetrics(SM_CXFRAME);
//	point.y = 30;
//
//	memDC.CreateCompatibleDC(NULL);
//	bmp.CreateCompatibleBitmap(pDC, point.x, point.y);
//	CBitmap *pOldBit = memDC.SelectObject(&bmp);
//
//
//	//背景颜色
//	memDC.FillSolidRect(0, 0, point.x, point.y, m_BKColor);
//
//	////贴图
//	/*CBitmap pic;
//	pic.LoadBitmap(IDB_BITMAP_BG);
//	ShowBitmap(&memDC,0,0, rtTitle.right, 30, pic);*/
//
//	pDC->BitBlt(0, 0, point.x, point.y, &memDC, 0, 0, SRCCOPY);
//
//	pOldBit->DeleteObject();
//	memDC.DeleteDC();
//	bmp.DeleteObject();
//
//}
//
//void CWeighingManagerDlg::DrawDlg(COLORREF colorValue)//背景色填充
//{
//	CDC memDC;
//	CRect wndRC;
//	CBitmap bmp;
//	CDC* pDC = GetWindowDC();
//
//	GetWindowRect(wndRC);
//	wndRC.OffsetRect(wndRC.Width(), wndRC.Height());
//	int nTitleHeight = GetSystemMetrics(SM_CYCAPTION);
//	int nborder = ::GetSystemMetrics(SM_CXFIXEDFRAME);
//
//	if (GetStyle()&WS_THICKFRAME)
//	{
//		int nborder = ::GetSystemMetrics(SM_CXBORDER);
//	}
//
//	memDC.CreateCompatibleDC(NULL);
//
//
//	//加载左边界
//	bmp.CreateCompatibleBitmap(pDC, nborder, wndRC.Height() - nTitleHeight);
//	memDC.SelectObject(&bmp);
//	memDC.FillSolidRect(0, 0, nborder, wndRC.Height() - nTitleHeight, colorValue);
//	pDC->BitBlt(0, nTitleHeight, nborder, wndRC.Height() - nTitleHeight, &memDC, 0, 0, SRCCOPY);
//	bmp.DeleteObject();
//
//	//加载右边界
//	bmp.CreateCompatibleBitmap(pDC, nborder, wndRC.Height() - nTitleHeight);
//	memDC.SelectObject(&bmp);
//	memDC.FillSolidRect(0, 0, nborder, wndRC.Height() - nTitleHeight, colorValue);
//	pDC->BitBlt(wndRC.Width() - nborder, nTitleHeight, nborder, wndRC.Height() - nTitleHeight, &memDC, 0, 0, SRCCOPY);
//	bmp.DeleteObject();
//
//
//
//	//加载底边
//	bmp.CreateCompatibleBitmap(pDC, wndRC.Width() - 2 * nborder, nborder);
//	memDC.SelectObject(&bmp);
//	memDC.FillSolidRect(0, 0, wndRC.Width() - 2 * nborder, nborder, colorValue);
//	pDC->BitBlt(nborder, wndRC.Height() - nborder, wndRC.Width() - 2 * nborder, wndRC.Height(), &memDC, 0, 0, SRCCOPY);
//	bmp.DeleteObject();
//
//
//	////加载标题栏
//	bmp.CreateCompatibleBitmap(pDC, wndRC.Width() - 2 * nborder, nborder);
//	memDC.SelectObject(&bmp);
//	memDC.FillSolidRect(0, 0, wndRC.Width(), nTitleHeight + nborder, colorValue);
//	pDC->BitBlt(0, 0, wndRC.Width(), nTitleHeight + nborder, &memDC, 0, 0, SRCCOPY);
//
//	bmp.DeleteObject();
//	memDC.DeleteDC();
//}
//
//void CWeighingManagerDlg::DrawDlg(UINT LeftBmp, UINT RightBmp, UINT ButtomBmp, UINT TitleBmp)
//{
//	CDC memDC;
//	CRect wndRC;
//	CBitmap bmp;
//
//	CDC* pDC = GetWindowDC();
//	GetWindowRect(wndRC);
//	wndRC.OffsetRect(wndRC.Width(), wndRC.Height());
//	int nTitleHeight = GetSystemMetrics(SM_CYCAPTION);
//	int nborder = ::GetSystemMetrics(SM_CXFIXEDFRAME);
//	if (GetStyle()&WS_THICKFRAME)
//	{
//		int nborder = ::GetSystemMetrics(SM_CXBORDER);
//	}
//
//	//加载左边界位图
//	bmp.LoadBitmap(LeftBmp);
//	BITMAP so_bmp;
//	bmp.GetBitmap(&so_bmp);
//	int nBmpWidth, nbmpHeight;
//	nBmpWidth = so_bmp.bmWidth;
//	nbmpHeight = so_bmp.bmHeight;
//
//	memDC.CreateCompatibleDC(pDC);
//	memDC.SelectObject(&bmp);
//	pDC->StretchBlt(0, nTitleHeight, nborder, wndRC.Height() - nTitleHeight, &memDC, 0, 0, nBmpWidth, nbmpHeight, SRCCOPY);
//	bmp.DeleteObject();
//
//	//加载右边界位图
//	bmp.LoadBitmap(RightBmp);
//	bmp.GetBitmap(&so_bmp);
//	nBmpWidth = so_bmp.bmWidth;
//	nbmpHeight = so_bmp.bmHeight;
//
//	memDC.SelectObject(&bmp);
//	pDC->StretchBlt(wndRC.Width() - nborder, nTitleHeight, nborder, wndRC.Height() - nTitleHeight, &memDC, 0, 0, nBmpWidth, nbmpHeight, SRCCOPY);
//	bmp.DeleteObject();
//
//	//加载底边位图
//	bmp.LoadBitmap(ButtomBmp);
//	bmp.GetBitmap(&so_bmp);
//	nBmpWidth = so_bmp.bmWidth;
//	nbmpHeight = so_bmp.bmHeight;
//
//	memDC.SelectObject(&bmp);
//	pDC->StretchBlt(nborder, wndRC.Height() - nborder, wndRC.Width() - 2 * nborder, nborder, &memDC, 0, 0, nBmpWidth, nbmpHeight, SRCCOPY);
//	bmp.DeleteObject();
//
//
//	//加载标题栏位图
//	bmp.LoadBitmap(TitleBmp);
//	bmp.GetBitmap(&so_bmp);
//	nBmpWidth = so_bmp.bmWidth;
//	nbmpHeight = so_bmp.bmHeight;
//
//	memDC.SelectObject(&bmp);
//	pDC->StretchBlt(0, 0, wndRC.Width(), nTitleHeight + nborder, &memDC, 0, 0, nBmpWidth, nbmpHeight, SRCCOPY);
//	bmp.DeleteObject();
//
//	memDC.DeleteDC();
//}

HBRUSH CWeighingManagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  在此更改 DC 的任何属性
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return (HBRUSH)m_BKBrush.GetSafeHandle();
	}
	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CWeighingManagerDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}