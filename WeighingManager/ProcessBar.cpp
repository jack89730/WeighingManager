// ProcessBar.cpp : 实现文件
//

#include "stdafx.h"
#include "WeighingManager.h"
#include "ProcessBar.h"
#include "afxdialogex.h"


// ProcessBar 对话框

IMPLEMENT_DYNAMIC(ProcessBar, CDialogEx)

ProcessBar::ProcessBar(CWnd* pParent /*=NULL*/)
	: CDialogEx(ProcessBar::IDD, pParent)
{

}

ProcessBar::~ProcessBar()
{
}

void ProcessBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_processbar);
}


BEGIN_MESSAGE_MAP(ProcessBar, CDialogEx)
END_MESSAGE_MAP()


// ProcessBar 消息处理程序
