// ControlPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "WeighingManager.h"
#include "ControlPanel.h"
#include "afxdialogex.h"


// CControlPanel 对话框

IMPLEMENT_DYNAMIC(CControlPanel, CDialogEx)

CControlPanel::CControlPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlPanel::IDD, pParent)
{

}

CControlPanel::~CControlPanel()
{
}

void CControlPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlPanel, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CControlPanel::OnBnClickedMfcbutton1)
END_MESSAGE_MAP()


// CControlPanel 消息处理程序


void CControlPanel::OnBnClickedMfcbutton1()
{
	// TODO:  在此添加控件通知处理程序代码
}
