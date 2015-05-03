#include "stdafx.h"
#include "OutputWin.h"

void OutputWin::SetClientHandle(int hClient)
{
	m_hClient = hClient;

	if (hClient)
	{
		m_bEnableShow = true;
		m_struInterV.SetInUse(true);
	}
	else
	{
		m_bEnableFrame = false;
		m_bEnableShow = false;
		m_struInterV.SetInUse(false);
	}
}

int OutputWin::GetClientHandle()
{
	return(m_hClient);
}

void OutputWin::ShowString()
{
	//只有稳定图像才需要取消
	if (m_bEnableFrame == false)
	{
		if (m_nCDNoString >= 0)
			m_nCDNoString--;
	}

	if (m_nCDNoString < 0)
		m_strResult[0] = 0;

	pEditResult->SetWindowText(m_strResult);
}

void OutputWin::UpdateString(const char *pStr)
{
	m_nCDNoString = 4;

	strcpy_s(m_strResult, MAX_LEN_STR, pStr);
}

void OutputWin::SetFrame(const unsigned char *pFrame, unsigned uWidth, unsigned uHeight, unsigned uPitch)
{
	EnterCriticalSection(&m_csFrame);

	m_bufFrame.SetFrame(pFrame, uWidth, uHeight, uPitch);
	//允许显示静态帧
	m_bEnableFrame = true;
	m_struInterV.SetInUse(true);
	LeaveCriticalSection(&m_csFrame);

	m_struInterV.Invalidate();
}

void OutputWin::AddPatch(const unsigned char *pPatch, unsigned uWidth, unsigned uHeight, unsigned uPitch,
	unsigned uX, unsigned uY)
{
	EnterCriticalSection(&m_csFrame);

	m_bufFrame.AddPatch(pPatch, uWidth, uHeight, uPitch, uX, uY);
	//允许显示静态帧
	LeaveCriticalSection(&m_csFrame);
}

unsigned char *OutputWin::GetFrame(unsigned &uWidth, unsigned &uHeight, unsigned &uPitch)
{
	unsigned char *pRT = NULL;
	EnterCriticalSection(&m_csFrame);
	if (m_bufFrame.m_pBuffer)
	{
		pRT = m_bufFrame.m_pBuffer;
		uWidth = m_bufFrame.m_uWidth;
		uHeight = m_bufFrame.m_uHeight;
		uPitch = m_bufFrame.m_uWidth * 3;
	}
	LeaveCriticalSection(&m_csFrame);

	return(pRT);
}

void OutputWin::ShowFrame()
{
	EnterCriticalSection(&m_csFrame);
	do
	{
		if (m_bEnableFrame == false)
			break;

		if (iGetDraw() == false)
			break;

		m_pDraw->DrawRGB24(m_bufFrame.m_pBuffer, m_bufFrame.m_uWidth, m_bufFrame.m_uHeight);

	} while (0);

	LeaveCriticalSection(&m_csFrame);
}

bool OutputWin::iGetDraw()
{
	if (m_pDraw == NULL)
	{
		m_pDraw = new GDIDraw();
		if (m_pDraw->InitHWND(m_struInterV.GetSafeHwnd()) == false)
		{
			delete m_pDraw;
			m_pDraw = NULL;
			return(false);
		}
	}

	return(true);
}

HWND OutputWin::GetHWnd()
{
	return(m_struInterV.GetSafeHwnd());
}