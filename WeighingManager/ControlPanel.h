#pragma once


// CControlPanel 对话框

class CControlPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CControlPanel)

public:
	CControlPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CControlPanel();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONTROLPANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
