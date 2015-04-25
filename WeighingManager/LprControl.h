#pragma once


// CLprControl 对话框

class CLprControl : public CDialogEx
{
	DECLARE_DYNAMIC(CLprControl)

public:
	CLprControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLprControl();

// 对话框数据
	enum { IDD = IDD_DIALOG_LPR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFind();
	void OnDevFound(const char *pStrDevName, const char *pStrIPAddr, WORD usPort1, WORD usPort2);
	afx_msg void OnBnClickedStopFind();
	afx_msg void OnBnClickedOpen();

	CString m_port;
	CString m_loginname;
	CString m_loginpwd;
	CString m_ip;
};
