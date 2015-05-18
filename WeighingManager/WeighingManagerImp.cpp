#include "stdafx.h"
#include "WeighingManagerImp.h"

CWeighingManagerImp::CWeighingManagerImp(CDialogEx *pView)
{
	m_pView = pView;
	//初始化串口，在下面加逻辑
	m_mapSerialPort[PORT_CONTROLPANEL1].InitPort(CWnd::FromHandle(m_pView->m_hWnd));

}
int CWeighingManagerImp::process_start()
{
	//MessageBox(NULL, _T("监控线圈1和线圈4"), _T("提示"), 0);
	return 0;
}

void CWeighingManagerImp::SetView(CDialogEx * pView)
{
	m_pView = pView;
}