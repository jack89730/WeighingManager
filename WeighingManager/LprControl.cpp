// LprControl.cpp : 实现文件
//

#include "stdafx.h"
#include "WeighingManager.h"
#include "LprControl.h"
#include "afxdialogex.h"


enum
{
	MSG_DEV_FOUND = 1,
};
// CLprControl 对话框

IMPLEMENT_DYNAMIC(CLprControl, CDialogEx)

CLprControl::CLprControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLprControl::IDD, pParent)
	, m_port(_T(""))
	, m_loginname(_T(""))
	, m_loginpwd(_T(""))
	, m_ip(_T(""))
{

}

CLprControl::~CLprControl()
{
}

void CLprControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_port);
	DDX_Text(pDX, IDC_EDIT2, m_loginname);
	DDV_MaxChars(pDX, m_loginname, 20);
	DDV_MaxChars(pDX, m_port, 10);
	DDX_Text(pDX, IDC_EDIT3, m_loginpwd);
	DDV_MaxChars(pDX, m_loginpwd, 20);
	DDX_Text(pDX, IDC_EDIT4, m_ip);
	DDV_MaxChars(pDX, m_ip, 20);
}


BEGIN_MESSAGE_MAP(CLprControl, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CLprControl::OnBnClickedFind)
	ON_BN_CLICKED(IDC_MFCBUTTON3, &CLprControl::OnBnClickedStopFind)
	ON_BN_CLICKED(IDC_BUTTON1, &CLprControl::OnBnClickedOpen)
END_MESSAGE_MAP()


void __stdcall FIND_DEVICE_CALLBACK(const char *pStrDevName, const char *pStrIPAddr,
	WORD usPort1, WORD usPort2, void *pUserData)
{
	CLprControl *pInstance = (CLprControl *)pUserData;
	pInstance->OnDevFound(pStrDevName, pStrIPAddr, usPort1, usPort2);
}

// CLprControl 消息处理程序
void CLprControl::OnBnClickedFind()
{
	CTreeCtrl *pTC = (CTreeCtrl *)GetDlgItem(IDC_TREE_DEV_FOUND);
	pTC->DeleteAllItems();

	VZLPRClient_StartFindDevice(FIND_DEVICE_CALLBACK, this);
}

void CLprControl::OnDevFound(const char *pStrDevName, const char *pStrIPAddr, WORD usPort1, WORD usPort2)
{
	char *pStrDev = new char[256];
	sprintf_s(pStrDev, 256, "%s:%d", pStrIPAddr, usPort1);
	PostMessage(WM_USER_MSG, (WPARAM)MSG_DEV_FOUND, (LPARAM)pStrDev);
}



void CLprControl::OnBnClickedStopFind()
{
	VZLPRClient_StopFindDevice();
}


void CLprControl::OnBnClickedOpen()
{
	UpdateData(true);

	//{
	//	//保存地址
	//	char strINI[MAX_PATH];
	//	sprintf_s(strINI, MAX_PATH, "%s/user.ini", m_strModuleDir);
	//	WritePrivateProfileString(m_strModuleName, "LastIP", strIP, strINI);
	//}

	VzLPRClientHandle hLPRClient = VzLPRClient_Open(m_ip, atoi(m_port), m_loginname, m_loginpwd);
	if (hLPRClient == NULL)
	{
		char str[128];
		sprintf_s(str, 128, "设备[%s:%s]打开失败，请确认设备已连接", m_ip, m_port);
		MessageBox(str);
		return;
	}

	//bool bFound = false;
	//for (unsigned i = 0; i<m_vDev.size(); i++)
	//{
	//	if (hLPRClient == m_vDev[i]->hLPRClient)
	//	{
	//		bFound = true;
	//		break;
	//	}
	//}

	//if (bFound == false)
	//{
	//	//保存到本地存储
	//	DeviceLPR *pDev = new DeviceLPR(strIP, hLPRClient);;
	//	m_vDev.push_back(pDev);

	//	//添加到设备列表
	//	HTREEITEM hItem = m_treeDeviceList.InsertItem(strIP, 0, 1);
	//	m_treeDeviceList.SetItemData(hItem, hLPRClient);
	//	m_treeDeviceList.SelectItem(hItem);
	//}

	//iChangeDlgImageItem(hLPRClient, DEV_STATUS_ONLINE);
}
