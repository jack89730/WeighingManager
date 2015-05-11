
// WeighingManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WeighingManager.h"
#include "WeighingManagerDlg.h"
#include "WeighingManagerImp.h"
#include "afxdialogex.h"
#include "ProcessBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define SAVE_LP2FILE	//定义该宏，输出测试所需的车牌识别结果
#endif

const int TIMER_RECORD_ID = 1001;
const int TIMER_RECORD_VALUE = 10 * 60 * 1000; // 定时器10分钟

enum
{
	DEV_STATUS_ONLINE,
	DEV_STATUS_OFFLINE,
};

enum
{
	IDM_REPORT = 10001,
    IDM_SET,
    IDM_LOGOUT
};

static void iImageListLoadIDB(int IDB_, CImageList *pImgList)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_);
	pImgList->Add(&bitmap, RGB(0, 0, 0));
}

static const char *iGetResultTypeName(VZ_LPRC_RESULT_TYPE eResultType)
{
	static const char *pStrTypeName[VZ_LPRC_RESULT_TYPE_NUM]
		= { "实时", "自动触发", "软件触发", "外部触发", "虚拟线圈", "多重触发" };

	return(pStrTypeName[eResultType]);
}

static const char *iGetResultTypeNameByBits(unsigned uBits)
{
	const int LenStr = 256;
	static char strTypes[LenStr];
	strTypes[0] = 0;
	if (uBits == 0)
	{
		strcat_s(strTypes, LenStr, iGetResultTypeName(VZ_LPRC_RESULT_REALTIME));
	}
	if (uBits & TRIGGER_TYPE_AUTO_BIT)
	{
		strcat_s(strTypes, LenStr, iGetResultTypeName(VZ_LPRC_RESULT_STABLE));
	}
	if (uBits & TRIGGER_TYPE_EXTERNAL_BIT)
	{
		strcat_s(strTypes, LenStr, iGetResultTypeName(VZ_LPRC_RESULT_IO_TRIGGER));
		strcat_s(strTypes, LenStr, "|");
	}
	if (uBits & TRIGGER_TYPE_SOFTWARE_BIT)
	{
		strcat_s(strTypes, LenStr, iGetResultTypeName(VZ_LPRC_RESULT_FORCE_TRIGGER));
		strcat_s(strTypes, LenStr, "|");
	}
	if (uBits & TRIGGER_TYPE_VLOOP_BIT)
	{
		strcat_s(strTypes, LenStr, iGetResultTypeName(VZ_LPRC_RESULT_VLOOP_TRIGGER));
		strcat_s(strTypes, LenStr, "|");
	}

	return(strTypes);
};

static const char *iGetDirString(int nDir)
{
	if (nDir == 3)
		return("向上");
	if (nDir == 4)
		return("向下");
	return(" ");
}

void __stdcall OnCommonNotify(VzLPRClientHandle handle, void *pUserData,
	VZ_LPRC_COMMON_NOTIFY eNotify, const char *pStrDetail)
{
	CWeighingManagerDlg *pInstance = (CWeighingManagerDlg *)pUserData;
	pInstance->OnCommonNotify0(handle, eNotify, pStrDetail);
}

int __stdcall OnPlateInfo(VzLPRClientHandle handle, void *pUserData,
	const TH_PlateResult *pResult, unsigned uNumPlates,
	VZ_LPRC_RESULT_TYPE eResultType,
	const VZ_LPRC_IMAGE_INFO *pImgFull,
	const VZ_LPRC_IMAGE_INFO *pImgPlateClip)
{
	CWeighingManagerDlg *pInstance = (CWeighingManagerDlg *)pUserData;
	pInstance->OnPlateInfo0(handle, pResult, uNumPlates, eResultType, pImgFull, pImgPlateClip);

	return(0);
}

int __stdcall OnQueryPlateInfo(VzLPRClientHandle handle, void *pUserData,
	const TH_PlateResult *pResult, unsigned uNumPlates,
	VZ_LPRC_RESULT_TYPE eResultType,
	const VZ_LPRC_IMAGE_INFO *pImgFull,
	const VZ_LPRC_IMAGE_INFO *pImgPlateClip)
{
	int mm = 0;

	return 0;
}

int __stdcall OnVideoFrame(VzLPRClientHandle handle, void *pUserData,
	WORD wVideoID, const VzYUV420P *pFrame)
{
	CWeighingManagerDlg *pInstance = (CWeighingManagerDlg *)pUserData;
	pInstance->OnVideoFrame0(handle, wVideoID, pFrame);

	return(0);
}

//void __stdcall OnSerialRecvData(int nSerialHandle, const unsigned char *pRecvData, unsigned uRecvSize, void *pUserData)
//{
//	CWeighingManagerDlg *pInstance = (CWeighingManagerDlg *)pUserData;
//	pInstance->OnSerialRecvData1(nSerialHandle, pRecvData, uRecvSize);
//}

void CALLBACK iOnViewerMouse(IV_EVENT eEvent, int x, int y, void *pUserData, int nId)
{
	CWeighingManagerDlg *pInstance = (CWeighingManagerDlg *)pUserData;
	pInstance->OnViewerMouse0(eEvent, x, y, nId);
}


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
	//m_BKColor = RGB(165, 201, 235);
	//m_BKBrush.CreateSolidBrush(RGB(165, 201, 235));
}

void CWeighingManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SHOW_1_1, m_winOut[0].m_struInterV);
	DDX_Control(pDX, IDC_STATIC_SHOW_1_2, m_winOut[1].m_struInterV);
	DDX_Control(pDX, IDC_STATIC_SHOW_2_1, m_winOut[2].m_struInterV);
	DDX_Control(pDX, IDC_STATIC_SHOW_2_2, m_winOut[3].m_struInterV);
	DDX_Control(pDX, IDC_TREE1, m_treeDeviceList);
}

BEGIN_MESSAGE_MAP(CWeighingManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(WM_APP + 1, OnMyMsgHandler)
	ON_COMMAND(IDC_TOOLBAR_BUTTON1, OnAutoStart)
	ON_COMMAND(IDM_SET, OnToolbarSet)
	ON_COMMAND(IDM_LOGOUT, OnOK)
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
	Init();
	InitStatusBar();

	InitImageList();
	InitToolBar();

	SetTimer(1, 1000, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWeighingManagerDlg::Init()
{
	//初始化串口，在下面加逻辑
	serialPort1.InitPort(CWnd::FromHandle(m_hWnd));

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//	_CrtSetBreakAlloc(100);
#endif

	//初始化结果输出窗口
	GetModuleFileName(NULL, m_strModuleDir, MAX_PATH);
	*strrchr(m_strModuleDir, '\\') = 0;
	strcpy_s(m_strModuleName, MAX_PATH, "VzLPRSDKDemoDlg");

	VZLPRClient_SetCommonNotifyCallBack(OnCommonNotify, this);

	int nIDCEditResult[MAX_OUTPUT_NUM] =
	{
		IDC_EDIT_RESULT_1_1, IDC_EDIT_RESULT_1_2,
		IDC_EDIT_RESULT_2_1, IDC_EDIT_RESULT_2_2
	};
	for (int i = 0; i<MAX_OUTPUT_NUM; i++)
	{
		m_winOut[i].pEditResult = (CEdit *)GetDlgItem(nIDCEditResult[i]);
		m_winOut[i].m_struInterV.SetID(i);
		m_winOut[i].m_struInterV.SetInteractCallback(iOnViewerMouse, this);
		m_winOut[i].m_struInterV.SetCallBackOnPaint(iOnIVPaint, this);
	}

	//为m_treeDeviceList建立图像列表
	m_pImageList = new CImageList();
	m_pImageList->Create(16, 16, ILC_COLOR32, 0, 2);
	iImageListLoadIDB(IDB_BMP_ONLINE, m_pImageList);
	iImageListLoadIDB(IDB_BMP_OFFLINE, m_pImageList);

	char strIP[32] = { 0 };
	{
		//使用上一次的IP地址
		char strINI[MAX_PATH];
		sprintf_s(strINI, MAX_PATH, "%s/user.ini", m_strModuleDir);
		GetPrivateProfileString(m_strModuleName, "LastIP", "", strIP, 32, strINI);
	}
	//SetDlgItemText(IDC_EDIT_IP, strIP);
	//SetDlgItemText(IDC_EDIT_PORT, "80");
	//SetDlgItemText(IDC_EDIT_USERNAME, "admin");
	//SetDlgItemText(IDC_EDIT_PASSWORD, "admin");

	//((CButton *)GetDlgItem(IDC_CHK_SAVE_STABLE))->SetCheck(BST_CHECKED);
	m_bSaveJPG = true;

	char strFilePath[MAX_PATH];

	sprintf_s(strFilePath, MAX_PATH, "%s/%s",
		m_strModuleDir, "Cap");

	if (!PathIsDirectory(strFilePath))
	{
		CreateDirectory(strFilePath, NULL);
	}

	m_strCommNotify[0] = 0;

	//m_dlgTriggerShow.Create(m_dlgTriggerShow.IDD, this);

	//添加输出端口类型
	/*CComboBox *pCmbOP = (CComboBox *)GetDlgItem(IDC_CMB_OUT_PORT_ID);
	pCmbOP->AddString("开关量 1");
	pCmbOP->AddString("开关量 2");
	pCmbOP->AddString("5V TLL 1");
	pCmbOP->AddString("5V TLL 2");*/

	/*CComboBox *pCmbBaudRate = (CComboBox *)GetDlgItem(IDC_CMB_SERIAL_PORT);
	pCmbBaudRate->AddString("Port1");
	pCmbBaudRate->AddString("Port2");
	pCmbBaudRate->SetCurSel(0);*/

	SetTimer(1000, 300, NULL);

	SetTimer(TIMER_RECORD_ID, TIMER_RECORD_VALUE, NULL);

#ifdef SAVE_LP2FILE
	time_t ttCurr;
	time(&ttCurr);
	tm tmCurr;
	localtime_s(&tmCurr, &ttCurr);
	sprintf_s(strFilePath, MAX_PATH, "%s/LP_%d%02d%02d_%2d%02d%02d.log",
		m_strModuleDir, tmCurr.tm_year + 1900, tmCurr.tm_mon + 1, tmCurr.tm_mday,
		tmCurr.tm_hour, tmCurr.tm_min, tmCurr.tm_sec);

	fopen_s(&m_pFSaveLP, strFilePath, "w");
#endif
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

void CWeighingManagerDlg::InitStatusBar()
{
	m_imglistStatusBar.Create(16, 16, ILC_COLOR24 | ILC_MASK, 5, 0);
	m_imglistStatusBar.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_imglistStatusBar.Add(AfxGetApp()->LoadIcon(IDI_ICON4));


	promptInfo[0] = "";
	promptInfo[1] = "控制板1串口";
	promptInfo[2] = "控制板2串口";
	promptInfo[3] = "摄像头1端口";
	promptInfo[4] = "摄像头2端口";
	promptInfo[5] = "LED屏幕串口";
	promptInfo[6] = "地磅串口";
	promptInfo[7] = "数据库状态";
	promptInfo[8] = theApp.m_pGlobalObject->GetUsername();

	m_StatusBar.Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0, 0, 0, 0), this, 0);
	int strPartDim[11] = { 0, 100, 200, 300, 400, 500, 600, 700, 800, 1020, -1 };
	m_StatusBar.SetParts(11, strPartDim);

	m_StatusBar.SetText(promptInfo[0], 0, 0);
	m_StatusBar.SetText(promptInfo[1], 1, 0);
	m_StatusBar.SetText(promptInfo[2], 2, 0);
	m_StatusBar.SetText(promptInfo[3], 3, 0);
	m_StatusBar.SetText(promptInfo[4], 4, 0);
	m_StatusBar.SetText(promptInfo[5], 5, 0);
	m_StatusBar.SetText(promptInfo[6], 6, 0);
	m_StatusBar.SetText(promptInfo[7], 7, 0);
	m_StatusBar.SetText(promptInfo[8], 8, 0);

	m_StatusBar.SetIcon(1, m_imglistStatusBar.ExtractIcon(1));
	m_StatusBar.SetIcon(2, m_imglistStatusBar.ExtractIcon(1));
	m_StatusBar.SetIcon(3, m_imglistStatusBar.ExtractIcon(1));
	m_StatusBar.SetIcon(4, m_imglistStatusBar.ExtractIcon(1));
	m_StatusBar.SetIcon(5, m_imglistStatusBar.ExtractIcon(1));
	m_StatusBar.SetIcon(6, m_imglistStatusBar.ExtractIcon(1));
	m_StatusBar.SetIcon(7, m_imglistStatusBar.ExtractIcon(1));
	//m_StatusBar.SetIcon(8, m_imglistStatusBar.ExtractIcon(1));
	//m_StatusBar.SetIcon(9, m_imglistStatusBar.ExtractIcon(1));
	//m_StatusBar.SetIcon(10, m_imglistStatusBar.ExtractIcon(0));
	
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	m_StatusBar.SetBkColor(RGB(180,180,180));   //状态栏背景色

	//要实现可拖动窗口的状态栏的跟随拖动，要在onsize中更新消息，如：	
	RECT   winrect;
	if (m_StatusBar.GetSafeHwnd() == NULL)
		return;
	this->GetClientRect(&winrect); //取得客户区大小
	m_StatusBar.MoveWindow((winrect.left + 50), (winrect.bottom - 30),
		(winrect.right - winrect.left - 100), 30, TRUE); //在适当的地方显示状态条 
}

void CWeighingManagerDlg::InitImageList()
{
	m_ImageListToolbar.Create(48, 48, ILC_COLOR24|ILC_MASK, 1, 1);
	m_ImageListToolbar.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageListToolbar.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_ImageListToolbar.Add(AfxGetApp()->LoadIcon(IDI_ICON5));
	m_ImageListToolbar.Add(AfxGetApp()->LoadIcon(IDI_ICON7));

	m_ImageListHotToolbar.Create(48, 48, ILC_COLOR24|ILC_MASK, 1, 1);
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
	//RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);  //显示工具栏

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, CWnd::reposDefault, NULL, NULL, true);
	m_ToolBar.GetToolBarCtrl().SetWindowPos(&CWnd::wndBottom, 10, 10, 100, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_ToolBar.ModifyStyle(0, TBSTYLE_TRANSPARENT | TBSTYLE_FLAT | TBSTYLE_WRAPABLE );//设置工具栏背景色透明    ---  (根据要求，可以自形选择)

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

LRESULT CWeighingManagerDlg::OnMyMsgHandler(WPARAM wparam, LPARAM lParam)
{
	CPoint pt;
	CMenu menu;

	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, IDM_REPORT, "报表");
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, IDM_SET, "设置");
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, IDM_LOGOUT, "退出");

	pt.x = (int)(short)LOWORD(lParam);
	pt.y = (int)(short)HIWORD(lParam);

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_HORPOSANIMATION
		, pt.x, pt.y, CWnd::FromHandle(m_hWnd));

	menu.DestroyMenu();
	return 0;
}

void CWeighingManagerDlg::OnAutoStart()
{
	CWeighingManagerImp cWMI;
	cWMI.process_start();
}

void CWeighingManagerDlg::OnToolbarSet()
{
	ProcessBar dlg1;
	dlg1.DoModal();
	CSysParametrSet dlg;
	dlg.DoModal();
}

void CWeighingManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == TIMER_RECORD_ID)
	{
		// 定时全部重新录像
		iReRecordAllVideo();
	}
	else
	{
		//轮询所有设备的状态
		for (unsigned i = 0; i<m_vDev.size(); i++)
		{
			BYTE cStatus = 0;
			VzLPRClient_IsConnected(m_vDev[i]->hLPRClient, &cStatus);
			if (cStatus == 1)
			{
				iChangeDlgImageItem(m_vDev[i]->hLPRClient, DEV_STATUS_ONLINE);
			}
			else
			{
				iChangeDlgImageItem(m_vDev[i]->hLPRClient, DEV_STATUS_OFFLINE);
			}
		}

		//更新界面内容
		for (unsigned i = 0; i<MAX_OUTPUT_NUM >> 1; i++)
		{
			m_winOut[i].ShowString();
		}

		if (m_nCDCleanCommNotify > 0)
		{
			m_nCDCleanCommNotify--;
			if (m_nCDCleanCommNotify == 0)
			{
				m_strCommNotify[0] = 0;
			}
			SetDlgItemText(IDC_EDIT_NOTIFY, m_strCommNotify);
		}

		//iUpdateSerialRecvInfo();
	}

	if (nIDEvent == 1)
	{
		CTime time;
		time = CTime::GetCurrentTime();
		m_StatusBar.SetText(time.Format("%Y") + ("年") + time.Format("%m") + ("月") + time.Format("%d") + ("日 ") + (" ") + time.Format("%H:%M:%S"), 9, 0);
	}

	CDialog::OnTimer(nIDEvent);

	CDialog::OnTimer(nIDEvent);
}

void CWeighingManagerDlg::iReRecordAllVideo()
{
	char szPath[MAX_PATH] = { 0 };

	int nSize = m_vecRecordHandle.size();
	for (int i = 0; i < nSize; i++)
	{
		VzLPRClientHandle hLPRClient = (VzLPRClientHandle)m_vecRecordHandle[i];
		if (hLPRClient != NULL)
		{
			CString strDevice = iGetDeviceItemText(hLPRClient);
			if (strDevice != "")
			{
				// 停止录像
				VzLPRClient_StopSaveRealData(hLPRClient);

				// 重新录像
				iRecordVideo(hLPRClient, strDevice);
			}
		}
	}
}

CString CWeighingManagerDlg::iGetDeviceItemText(VzLPRClientHandle hLPRClient)
{
	CString strDevice;

	HTREEITEM hItem = m_treeDeviceList.GetRootItem();
	VzLPRClientHandle hCurClient;

	while (hItem != NULL)
	{
		hCurClient = m_treeDeviceList.GetItemData(hItem);
		if (hCurClient == hLPRClient)
		{
			strDevice = m_treeDeviceList.GetItemText(hItem);
			break;
		}

		hItem = m_treeDeviceList.GetNextItem(hItem, TVGN_NEXT);
	}

	return strDevice;
}

void CWeighingManagerDlg::iRecordVideo(VzLPRClientHandle hLPRClient, CString strDevice)
{
	COleDateTime dtNow = COleDateTime::GetCurrentTime();

	char strFilePath[MAX_PATH] = { 0 };
	sprintf_s(strFilePath, MAX_PATH, "%s\\Video", m_strModuleDir);

	if (!PathIsDirectory(strFilePath))
	{
		CreateDirectory(strFilePath, NULL);
	}

	char szPath[MAX_PATH] = { 0 };
	sprintf_s(szPath, sizeof(szPath), "%s\\%s_%d%02d%02d%02d%02d%02d.avi", strFilePath, strDevice.GetBuffer(0), dtNow.GetYear(), dtNow.GetMonth(), dtNow.GetDay(),
		dtNow.GetHour(), dtNow.GetMinute(), dtNow.GetSecond());

	VzLPRClient_SaveRealData(hLPRClient, szPath);
}

void CWeighingManagerDlg::iChangeDlgImageItem(VzLPRClientHandle handle, int imageID)
{
	HTREEITEM hRoot = m_treeDeviceList.GetRootItem();
	while (hRoot)
	{
		if (handle == m_treeDeviceList.GetItemData(hRoot))
		{
			m_treeDeviceList.SetItemImage(hRoot, imageID, imageID);
			return;
		}
		hRoot = m_treeDeviceList.GetNextItem(hRoot, TVGN_NEXT);
	}
}

void CALLBACK CWeighingManagerDlg::iOnIVPaint(int nID, bool bActive, bool bInUse, void *pUserData)
{
	CWeighingManagerDlg *pInstance = (CWeighingManagerDlg *)pUserData;
	pInstance->iOnIVPaint1(nID, bActive, bInUse);
}

void CWeighingManagerDlg::iOnIVPaint1(int nID, bool bActive, bool bInUse)
{
	m_winOut[nID].ShowString();

	if (bInUse)
	{
		if (nID >= (MAX_OUTPUT_NUM >> 1))
			m_winOut[nID].ShowFrame();
	}
}

void CWeighingManagerDlg::OnPlateInfo0(VzLPRClientHandle handle,
	const TH_PlateResult *pResult, unsigned uNumPlates,
	VZ_LPRC_RESULT_TYPE eResultType,
	const VZ_LPRC_IMAGE_INFO *pImgFull,
	const VZ_LPRC_IMAGE_INFO *pImgPlateClip)
{
	if (handle == NULL || pResult == NULL || uNumPlates == 0)
		return;

	DeviceLPR *pDev = iGetLocalDev(handle);

	char strMsg[MAX_LEN_STR] = { 0 };
	for (unsigned i = 0; i<uNumPlates; i++)
	{
		char strTmp[MAX_LEN_STR];

		sprintf_s(strTmp, MAX_LEN_STR, " [%s:%s （宽度=%d)] %s %d/%02d/%02d %02d:%02d:%02d",
			iGetResultTypeNameByBits(pResult[i].uBitsTrigType), pResult[i].license,
			pResult[i].rcLocation.right - pResult[i].rcLocation.left, iGetDirString(pResult[i].nDirection),
			pResult[i].struBDTime.bdt_year, pResult[i].struBDTime.bdt_mon, pResult[i].struBDTime.bdt_mday,
			pResult[i].struBDTime.bdt_hour, pResult[i].struBDTime.bdt_min, pResult[i].struBDTime.bdt_sec);
		strcat_s(strMsg, MAX_LEN_STR, strTmp);
	}

	OutputWin *pOutWin = eResultType == VZ_LPRC_RESULT_REALTIME
		? iGetOutputWinByIdx(pDev->GetWinIdx()) : iGetStableOutputWinByIdx(pDev->GetWinIdx());

	if (pOutWin)
	{
		pOutWin->UpdateString(strMsg);
		if (pImgFull)
		{
			pOutWin->SetFrame(pImgFull->pBuffer, pImgFull->uWidth, pImgFull->uHeight, pImgFull->uPitch);
			unsigned uYOffset = 4;
			for (unsigned i = 0; i<uNumPlates; i++)
			{
				pOutWin->AddPatch(pImgPlateClip[i].pBuffer, pImgPlateClip[i].uWidth,
					pImgPlateClip[i].uHeight, pImgPlateClip[i].uPitch, 4, uYOffset);
				uYOffset += pImgPlateClip[i].uHeight;
			}
		}
	}

	if (m_bSaveJPG)
	{
		iSaveJPEG(pImgFull, eResultType, pResult[0].license);
	}

	//保存触发类型的结果到文本（非实时）
	if (m_pFSaveLP && eResultType != VZ_LPRC_RESULT_REALTIME)
	{
		if (uNumPlates > 0)
		{
			for (unsigned i = 0; i<uNumPlates; i++)
			{
				fprintf(m_pFSaveLP, "%s ", pResult[0].license);
			}
			fprintf(m_pFSaveLP, "\n");
		}
		fflush(m_pFSaveLP);
	}
}

void CWeighingManagerDlg::OnVideoFrame0(VzLPRClientHandle handle, WORD wVideoID, const VzYUV420P *pFrame)
{
	OutputWin *pWinOut = iGetOutputWinByIdx(wVideoID);
	if (pWinOut == NULL)
		return;
}

void CWeighingManagerDlg::OnViewerMouse0(IV_EVENT eEvent, int x, int y, int nId)
{
	if (nId == 2 || nId == 3)
		nId -= 2;

	OutputWin &winOut = m_winOut[nId];
	OutputWin &winOut2 = m_winOut[nId + 2];

	if (eEvent == IV_EVENT_L_BTN_DOWN)
	{
		m_nIdxWinCurrSelected = nId;
		winOut.m_struInterV.SetActive(true);
		winOut2.m_struInterV.SetActive(true);
		for (int i = 0; i<MAX_OUTPUT_NUM; i++)
		{
			if (i == nId || i == nId + 2)
				continue;
			m_winOut[i].m_struInterV.SetActive(false);
		}
	}
}

void CWeighingManagerDlg::OnCommonNotify0(VzLPRClientHandle handle,
	VZ_LPRC_COMMON_NOTIFY eNotify, const char *pStrDetail)
{
	char strDetail[MAX_LEN_STR];

	if (pStrDetail)
		sprintf_s(strDetail, MAX_LEN_STR, "[0x%08x]Noitfy=%d: %s", handle, eNotify, pStrDetail);
	else
		sprintf_s(strDetail, MAX_LEN_STR, "[0x%08x]Noitfy=%d", handle, eNotify);

	iSetCommonInfo(strDetail);
}

DeviceLPR *CWeighingManagerDlg::iGetLocalDev(VzLPRClientHandle hLPRClient)
{
	for (unsigned i = 0; i<m_vDev.size(); i++)
	{
		if (m_vDev[i]->hLPRClient == hLPRClient)
			return(m_vDev[i]);
	}

	return(NULL);
}

OutputWin *CWeighingManagerDlg::iGetOutputWinByIdx(int nIdxWin)
{
	if (nIdxWin >= 0 && nIdxWin < (MAX_OUTPUT_NUM >> 1))
		return(m_winOut + nIdxWin);

	return(NULL);
}

OutputWin *CWeighingManagerDlg::iGetStableOutputWinByIdx(int nIdx)
{
	OutputWin *pWin = iGetOutputWinByIdx(nIdx);
	if (pWin == NULL)
		return(NULL);

	return(pWin + (MAX_OUTPUT_NUM >> 1));
}

//找到一个合适的输出窗口：
int CWeighingManagerDlg::iGetOutputWinIndex(VzLPRClientHandle hLPRClient)
{
	//如果未指定客户端句柄
	if (hLPRClient == NULL)
	{
		//如果以选中某个窗口，则直接使用该窗口
		if (m_nIdxWinCurrSelected >= 0)
			return(m_nIdxWinCurrSelected);

		//如果未选中任一窗口，则使用未被占用的一个空闲窗口
		for (int i = 0; i<MAX_OUTPUT_NUM >> 1; i++)
		{
			if (m_winOut[i].GetClientHandle() == NULL)
				return(i);
		}
	}
	else	//如果以指定一个客户端句柄，则使用已和该客户端句柄关联的窗口
	{
		for (int i = 0; i<MAX_OUTPUT_NUM >> 1; i++)
		{
			if (m_winOut[i].GetClientHandle() == hLPRClient)
				return(i);
		}
	}

	return(-1);
}

void CWeighingManagerDlg::iSetCommonInfo(const char *pStrInfo)
{
	if (pStrInfo == NULL)
		return;

	m_nCDCleanCommNotify = 10;
	strcpy_s(m_strCommNotify, MAX_LEN_STR, pStrInfo);
}

void CWeighingManagerDlg::iSaveJPEG(const VZ_LPRC_IMAGE_INFO *pImgInfo, VZ_LPRC_RESULT_TYPE eType, const char *pStrLicense)
{
	char strFilePath[MAX_PATH];

	sprintf_s(strFilePath, MAX_PATH, "%s/%s/%s",
		m_strModuleDir, "Cap", iGetResultTypeName(eType));

	if (!PathIsDirectory(strFilePath))
	{
		CreateDirectory(strFilePath, NULL);
	}

	char strFileName[MAX_PATH];

	CTime time = CTime::GetCurrentTime();
	sprintf_s(strFileName, MAX_PATH, "%s/%02d%02d%02d_%d%02d%02d_%s.jpg",
		strFilePath,
		time.GetYear() % 100, time.GetMonth(), time.GetDay(),
		time.GetHour(), time.GetMinute(), time.GetSecond(),
		pStrLicense);

	if (pImgInfo)
	{
		VzLPRClient_ImageSaveToJpeg(pImgInfo, strFileName, 80);
	}
}