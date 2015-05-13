#include "stdafx.h"
#include "WeighingManagerImp.h"

CWeighingManagerImp::CWeighingManagerImp(CDialogEx *pView)
{
	m_pView = pView;
}
int CWeighingManagerImp::process_start()
{
	MessageBox(NULL, _T("开始自动运行"), _T("提示"), 0);
	return 0;
}

void CWeighingManagerImp::SetView(CDialogEx * pView)
{
	m_pView = pView;
}