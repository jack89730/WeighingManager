#ifndef __WEIGHINGMANAGERIMP_H__
#define __WEIGHINGMANAGERIMP_H__

#include <map>

// 此类为业务实现类，实现称重管理系统的自动化功能
class CWeighingManagerImp
{
public:
	CWeighingManagerImp(){};
	CWeighingManagerImp(CDialogEx *pView);
	~CWeighingManagerImp(){}

public:
	int Init(int iPort);
	int process_start();
	int process_stop();
	void SetView(CDialogEx * pView);
	int OnHandleXXX();

protected:
	CDialogEx * m_pView;
	enum E_PORT
	{
		PORT_CONTROLPANEL1 = 1,	// 控制板1
		PORT_CONTROLPANEL2 = 2,	// 控制板2
		PORT_POUNDSCONTROL = 3,	// 地磅
		PORT_LEDCONTROL = 4		// LED显示屏
	};
	map<int, CSerialPort> m_mapSerialPort;
	//CSerialPort serialPort1;
	//CSerialPort serialPort2;
	//CSerialPort serialPort3;
	//CSerialPort serialPort4;
	//CSerialPort serialPort5;
};

#endif
