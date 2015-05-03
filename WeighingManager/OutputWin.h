#ifndef __OUTPUT_WIN_H__
#define __OUTPUT_WIN_H__

#include "GDIDraw.h"
#include "InteractViewer.h"
#include "afxwin.h"

#define MAX_LEN_STR		256

class _FRAME_BUFFER
{
public:
	_FRAME_BUFFER()
		: m_pBuffer(NULL), m_uSizeBuffer(0), m_uWidth(0), m_uHeight(0)
	{
	}
	~_FRAME_BUFFER()
	{
		if (m_pBuffer)
			free(m_pBuffer);
	}

	bool SetFrame(const unsigned char *pSrc, unsigned uWidth, unsigned uHeight, unsigned uPitch)
	{
		if (uWidth > m_uWidth || m_uHeight > uHeight)
		{
			if (m_pBuffer)
				free(m_pBuffer);
			m_uSizeBuffer = uPitch * uHeight;
			m_pBuffer = (unsigned char *)malloc(m_uSizeBuffer);
		}
		if (m_pBuffer == NULL)
			return(false);

		unsigned char *pDst = m_pBuffer;
		for (unsigned i = 0; i<uHeight; i++)
		{
			memcpy(pDst, pSrc, uWidth * 3);
			pSrc += uPitch;
			pDst += uWidth * 3;
		}

		m_uWidth = uWidth;
		m_uHeight = uHeight;

		return(true);
	}

	bool AddPatch(const unsigned char *pPatch, unsigned uWidth, unsigned uHeight, unsigned uPitch,
		unsigned uX, unsigned uY)
	{
		if (uX + uWidth > m_uWidth || uY + uHeight > m_uHeight || uX < 0 || uY < 0)
			return(false);

		if (m_pBuffer == NULL)
			return(false);

		unsigned uDstPitch = m_uWidth * 3;
		unsigned char *pDst = m_pBuffer + uY * uDstPitch + uX * 3;
		for (unsigned i = 0; i<uHeight; i++)
		{
			memcpy(pDst, pPatch, uPitch);
			pPatch += uPitch;
			pDst += uDstPitch;
		}

		return(true);
	}

public:
	unsigned char *m_pBuffer;
	unsigned m_uSizeBuffer;
	unsigned m_uWidth;
	unsigned m_uHeight;
};

class OutputWin
{
public:
	OutputWin()
		: pEditResult(NULL), m_bEnableShow(false)
		, m_hClient(0), m_bEnableFrame(false)
		, m_pDraw(NULL), nPlayHandle(0)
	{
		InitializeCriticalSection(&m_csFrame);
		m_strResult[0] = 0;
	}

	~OutputWin()
	{
		DeleteCriticalSection(&m_csFrame);
		if (m_pDraw)
			delete m_pDraw;
	}

	void SetClientHandle(int hClient);
	int GetClientHandle();

	void UpdateString(const char *pStr);

	void SetFrame(const unsigned char *pFrame, unsigned uWidth, unsigned uHeight, unsigned uPitch);

	void AddPatch(const unsigned char *pPatch, unsigned uWidth, unsigned uHeight, unsigned uPitch,
		unsigned uX, unsigned uY);

	unsigned char *GetFrame(unsigned &uWidth, unsigned &uHeight, unsigned &uPitch);

	void ShowString();

	void ShowFrame();

	HWND GetHWnd();

private:
	bool iGetDraw();

public:
	InteractViewer m_struInterV;
	CEdit *pEditResult;

	int nPlayHandle;

private:
	bool m_bEnableShow;
	int m_hClient;

	bool m_bEnableFrame;
	_FRAME_BUFFER m_bufFrame;

	CRITICAL_SECTION m_csFrame;

	int m_nCDNoString;
	char m_strResult[MAX_LEN_STR];

	GDIDraw *m_pDraw;
};
#endif