#include "stdafx.h"
#include "WeighingManagerImp.h"

CWeighingManagerImp::CWeighingManagerImp(CDialogEx *pView)
{
	m_pView = pView;
	//初始化串口，在下面加逻辑
}

int CWeighingManagerImp::Init(map<int, int>& mapPortType)
{
	m_mapSerialPort.clear();
	m_mapPortType.clear();
	map<int, int>::iterator it;
	for (it = mapPortType.begin(); it != mapPortType.end(); it++)
	{
		m_mapSerialPort[it->first].InitPort(CWnd::FromHandle(m_pView->m_hWnd), it->first);
		//CSerialPort* pPort = NULL;
		//pPort->InitPort(CWnd::FromHandle(m_pView->m_hWnd), it->first);
		//m_mapSerialPort.push_back(it->first, it->second, pPort);
	}
	m_mapPortType = mapPortType;

	return 0;
}

int CWeighingManagerImp::AddPort(int iPortID, int iType)
{
	m_mapSerialPort[iPortID].InitPort(CWnd::FromHandle(m_pView->m_hWnd), iPortID);
	m_mapSerialPort[iPortID].StartMonitoring();
	m_mapPortType[iPortID] = iType;
	//CSerialPort* pPort = NULL;
	//pPort->InitPort(CWnd::FromHandle(m_pView->m_hWnd), iPortID);
	//m_mapSerialPort.push_back(iPortID, iType, pPort);
	return 0;
}

int CWeighingManagerImp::process_start()
{
	std::map<int, CSerialPort>::iterator it;
	for (it = m_mapSerialPort.begin(); it != m_mapSerialPort.end(); it++)
	{
		it->second.StartMonitoring();
	}
	//double_map<int, int, CSerialPort>::iterator2 it;
	//for (it = m_mapSerialPort.begin2(); it != m_mapSerialPort.end2(); it++)
	//{
	//	it->second.StartMonitoring();
	//}
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
	//double_map<int, int, CSerialPort>::iterator2 it;
	//for (it = m_mapSerialPort.begin2(); it != m_mapSerialPort.end2(); it++)
	//{
	//	it->second.StopMonitoring();
	//	it->second.ClosePort();
	//}
	return 0;
}

void CWeighingManagerImp::SetView(CDialogEx * pView)
{
	m_pView = pView;
}

int CWeighingManagerImp::ring_detected(WPARAM wParam, LPARAM lParam)
{
	int iPortType = m_mapPortType[lParam];
	switch (iPortType)
	//double_map<int, int, CSerialPort>::iterator1 iter1;
	//double_map<int, int, CSerialPort>::iterator2 iter2;
	//m_mapSerialPort.find(*lParam, iter1, iter2);
	//switch (iter1->second)
	{
	case PORT_CONTROLPANEL1:
		MessageBox(NULL, _T("端口类型：控制板1"), _T("提示"), 0);
		break;
	case PORT_CONTROLPANEL2:
		MessageBox(NULL, _T("端口类型：控制板2"), _T("提示"), 0);
		break;
	default:
		MessageBox(NULL, _T("不能识别的参数类型"), _T("警告"), 0);
		break;
	}
	return 0;
}