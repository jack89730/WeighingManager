#include "stdafx.h"
#include "WeighingManagerImp.h"

CWeighingManagerImp::CWeighingManagerImp(CDialogEx *pView)
{
	m_pView = pView;
	//初始化串口，在下面加逻辑
}

int CWeighingManagerImp::Init(map<int, int>& mapSerialPort)
{
	 map<int, int>::iterator it;
	for ( it= mapSerialPort.begin(); it != mapSerialPort.end(); it++)
	{
		m_mapSerialPort[it->first].InitPort(CWnd::FromHandle(m_pView->m_hWnd), it->first);
	}

	return 0;
}

int CWeighingManagerImp::AddPort(int iPort)
{
	m_mapSerialPort[iPort].InitPort(CWnd::FromHandle(m_pView->m_hWnd), iPort);
	m_mapSerialPort[iPort].StartMonitoring();
	return 0;
}

int CWeighingManagerImp::process_start()
{
	std::map<int, CSerialPort>::iterator it;
	for (it = m_mapSerialPort.begin(); it != m_mapSerialPort.end(); it++)
	{
		it->second.StartMonitoring();
	}
	return 0;
}

int CWeighingManagerImp::process_stop()
{
	std::map<int, CSerialPort>::iterator it;
	for (it = m_mapSerialPort.begin(); it != m_mapSerialPort.end(); it++)
	{
		it->second.StopMonitoring();
		it->second.ClosePort();
	}
	return 0;
}

void CWeighingManagerImp::SetView(CDialogEx * pView)
{
	m_pView = pView;
}