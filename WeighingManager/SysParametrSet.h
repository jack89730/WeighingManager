#pragma once

//#include "MyTab.h"
#include "afxcmn.h"
#include "ControlPanel.h"
#include "LprControl.h"
// CSysParametrSet 对话框

class CSysParametrSet : public CDialogEx
{
	DECLARE_DYNAMIC(CSysParametrSet)

public:
	CSysParametrSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysParametrSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//CMyTab	m_TabParamter;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_TabParamter;
	CControlPanel m_pControlPanel;
	CLprControl   m_pLprControl;
};
