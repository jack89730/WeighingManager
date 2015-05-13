#ifndef __WEIGHINGMANAGERIMP_H__
#define __WEIGHINGMANAGERIMP_H__

// 此类为业务实现类，实现称重管理系统的自动化功能
class CWeighingManagerImp
{
public:
	CWeighingManagerImp(){};
	CWeighingManagerImp(CDialogEx *pView);
	~CWeighingManagerImp(){}

public:
	int process_start();
	void SetView(CDialogEx * pView);
	int OnHandleXXX();

protected:
	CDialogEx * m_pView;
};

#endif
