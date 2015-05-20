#include "stdafx.h"
#include "WeighingManagerImp.h"

CWeighingManagerImp::CWeighingManagerImp(CDialogEx *pView)
{
	m_pView = pView;
	//初始化串口，在下面加逻辑
}

int CWeighingManagerImp::Init(int iPort)
{
	m_mapSerialPort[PORT_CONTROLPANEL1].InitPort(CWnd::FromHandle(m_pView->m_hWnd), iPort);
	return 0;
}

int CWeighingManagerImp::process_start()
{
	//MessageBox(NULL, _T("监控线圈1和线圈4"), _T("提示"), 0);
	m_mapSerialPort[PORT_CONTROLPANEL1].StartMonitoring();
	return 0;
}

int CWeighingManagerImp::process_stop()
{
	m_mapSerialPort[PORT_CONTROLPANEL1].StopMonitoring();
	m_mapSerialPort[PORT_CONTROLPANEL1].ClosePort();
	return 0;
}

void CWeighingManagerImp::SetView(CDialogEx * pView)
{
	m_pView = pView;
}