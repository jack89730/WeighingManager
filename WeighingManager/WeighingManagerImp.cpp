#include "stdafx.h"
#include "WeighingManagerImp.h"

CWeighingManagerImp::CWeighingManagerImp(CDialogEx *pView) :m_iSts(STS_INACTIVE)
{
	m_pView = pView;
	//初始化串口，在下面加逻辑
	m_mapSerialPort[PORT_CONTROLPANEL1].InitPort(CWnd::FromHandle(m_pView->m_hWnd));

}
int CWeighingManagerImp::process_start()
{
	m_iSts = !m_iSts;
	if (m_iSts == E_STS::STS_ACTIVE) MessageBox(NULL, _T("开始自动运行"), _T("提示"), 0);
	else MessageBox(NULL, _T("停止自动运行"), _T("提示"), 0);
	while (m_iSts == STS_ACTIVE)
	{
		MessageBox(NULL, _T("监控线圈1、线圈4"), _T("提示"), 0);
		//m_mapSerialPort[PORT_CONTROLPANEL1].StartMonitoring();
	}
	return 0;
}

void CWeighingManagerImp::SetView(CDialogEx * pView)
{
	m_pView = pView;
}