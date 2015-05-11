
// WeighingManagerDlg.h : 头文件
//

#pragma once
#include "CToolBarEx.h"
#include "SysParametrSet.h"
#include "OutputWin.h"
#include <map>
#include <vector>
#include "afxcmn.h"

#define MAX_OUTPUT_NUM	4
#define MAX_SERIAL_RECV_SIZE	48

class DeviceLPR
{
public:
	DeviceLPR(const char *pStrIP, VzLPRClientHandle hLPRClient)
		: hLPRClient(hLPRClient), nIdxWin(-1)
	{
		strcpy_s(strIP, 256, pStrIP);
		memset(nSerialHandle, 0, sizeof(int)* 2);
	}

	void SetWinIdx(int _nIdxWin)
	{
		nIdxWin = _nIdxWin;
	}

	int GetWinIdx() const
	{
		return(nIdxWin);
	}

public:
	char strIP[256];
	VzLPRClientHandle hLPRClient;
	int nIdxWin;
	int nSerialHandle[2];
};

#define LICENSE_LEN	16
typedef struct
{
	char strLicense[LICENSE_LEN];
	unsigned uCarID;
}
CarInfo;


// CWeighingManagerDlg 对话框
class CWeighingManagerDlg : public CDialogEx
{
// 构造
public:
	CWeighingManagerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WEIGHINGMANAGER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnMyMsgHandler(WPARAM, LPARAM);
public:
	void OnPlateInfo0(VzLPRClientHandle handle,
		const TH_PlateResult *pResult, unsigned uNumPlates,
		VZ_LPRC_RESULT_TYPE eResultType,
		const VZ_LPRC_IMAGE_INFO *pImgFull,
		const VZ_LPRC_IMAGE_INFO *pImgPlateClip);
	void OnViewerMouse0(IV_EVENT eEvent, int x, int y, int nId);
	void OnVideoFrame0(VzLPRClientHandle handle, WORD wVideoID, const VzYUV420P *pFrame);
	void OnCommonNotify0(VzLPRClientHandle handle,VZ_LPRC_COMMON_NOTIFY eNotify, const char *pStrDetail);
	
private:
	void OnAutoStart();
	void OnToolbarSet();
	void InitImageList();
	BOOL InitToolBar();
	void InitStatusBar();
	void iReRecordAllVideo();
	CString iGetDeviceItemText(VzLPRClientHandle hLPRClient);
	void iRecordVideo(VzLPRClientHandle hLPRClient, CString strDevice);
	void iChangeDlgImageItem(VzLPRClientHandle handle, int imageID);
	void iOnIVPaint1(int nID, bool bActive, bool bInUse);
	DeviceLPR* iGetLocalDev(VzLPRClientHandle hLPRClient);
	static void CALLBACK iOnIVPaint(int nID, bool bActive, bool bInUse, void *pUserData);
	OutputWin * iGetOutputWinByIdx(int nIdxWin);
	OutputWin * iGetStableOutputWinByIdx(int nIdx);
	int iGetOutputWinIndex(VzLPRClientHandle hLPRClient);
	void iSetCommonInfo(const char *pStrInfo);
	void iSaveJPEG(const VZ_LPRC_IMAGE_INFO *pImgInfo, VZ_LPRC_RESULT_TYPE eType, const char *pStrLicense);
	void Init();

protected:
	HICON m_hIcon;
private:
	CImageList       m_ImageListHotToolbar;//使工具栏图标显示256色
	CImageList       m_ImageListToolbar;
	CToolBarEx       m_ToolBar;
	CStatusBarCtrl   m_StatusBar;
	CString          promptInfo[9];
	CImageList       m_imglistStatusBar;
	//CDrawAppearance m_drawApe;
	//COLORREF        m_BKColor;
	//CBrush          m_BKBrush;

	OutputWin m_winOut[MAX_OUTPUT_NUM];
	CImageList *m_pImageList;
	int m_nIdxWinCurrSelected;
	vector <DeviceLPR *> m_vDev;
	char m_strModuleDir[MAX_PATH];
	char m_strModuleName[MAX_PATH];
	bool m_bSaveJPG;
	//用于临时存储回调中的字符串，避免回调中直接使用界面控件
	int m_nCDCleanCommNotify;
	char m_strCommNotify[MAX_LEN_STR];
	char m_strResult[MAX_LEN_STR];
	unsigned char m_bufSerialRecv[MAX_SERIAL_RECV_SIZE];
	unsigned m_uSizeSerialRecv;
	int m_nLastSerialHandle;
	int m_nLastLEDLevel;
	vector<int> m_vecRecordHandle;
	FILE *m_pFSaveLP;
public:
	CTreeCtrl m_treeDeviceList;
	CSerialPort serialPort1;
	CSerialPort serialPort2;
	CSerialPort serialPort3;
	CSerialPort serialPort4;
	CSerialPort serialPort5;
};
