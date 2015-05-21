#ifndef __WEIGHINGMANAGERIMP_H__
#define __WEIGHINGMANAGERIMP_H__

#include <map>
//#include "double_map.h"

enum E_PORT_TYPE
{
	PORT_CONTROLPANEL1 = 1,	// 控制板1
	PORT_CONTROLPANEL2 = 2,	// 控制板2
	PORT_POUNDSCONTROL = 3,	// 地磅
	PORT_LEDCONTROL = 4		// LED显示屏
};

// 此类为业务实现类，实现称重管理系统的自动化功能
class CWeighingManagerImp
{
public:
	CWeighingManagerImp(){};
	CWeighingManagerImp(CDialogEx *pView);
	~CWeighingManagerImp(){}

public:
	int Init(map<int, int>& mapPortType);
	int process_start();
	int process_stop();
	void SetView(CDialogEx * pView);
	int  AddPort(int iPortID, int iType);
	int OnHandleXXX();
	int ring_detected(WPARAM wParam, LPARAM lParam);

protected:
	CDialogEx * m_pView;
	//double_map<int, int, CSerialPort*> m_mapSerialPort;	// key: 端口号, value1: 硬件类型, value2: 串口类变量
	map<int,CSerialPort> m_mapSerialPort;
	map<int, int> m_mapPortType;
};

#endif
