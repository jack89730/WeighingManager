#pragma once


typedef enum IV_EVENT
{
	IV_ENVET_MOUSE_MOVE = 1,
	IV_EVENT_L_BTN_DOWN,
	IV_EVENT_L_BTN_UP,
}
IV_EVENT;

typedef void (CALLBACK *IV_Callback)(IV_EVENT eEvent, int x, int y, void *pUserData, int nId);
typedef void (CALLBACK *IV_CallBackOnPaint)(int nID, bool bActive, bool bInUse, void *pUserData);

// InteractViewer

class InteractViewer : public CStatic
{
	DECLARE_DYNAMIC(InteractViewer)

public:
	InteractViewer();
	virtual ~InteractViewer();

	void SetID(int nId)
	{
		m_nId = nId;
	}

	void SetActive(bool bActive)
	{
		m_bActived = bActive;
		Invalidate();
	}

	void SetInUse(bool bInUse)
	{
		m_bInUse = bInUse;
	}

	void SetInteractCallback(IV_Callback cbFunc, void *pUserData)
	{
		m_cbFunc = cbFunc;
		m_pUserData = pUserData;
	}

	void SetCallBackOnPaint(IV_CallBackOnPaint funcOnPaint, void *pUserData)
	{
		m_funcOnPaint = funcOnPaint;
		m_pUserDataOnPaint = pUserData;
	}

	void CleanWindow();

protected:
	void iSetEvent(IV_EVENT eEvent, int x, int y);

protected:
	IV_Callback m_cbFunc;
	void *m_pUserData;
	int m_nId;
	bool m_bActived;
	bool m_bInUse;

	IV_CallBackOnPaint m_funcOnPaint;
	void *m_pUserDataOnPaint;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};


