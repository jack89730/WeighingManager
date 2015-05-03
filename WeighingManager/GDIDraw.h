//»æÍ¼
#ifndef _GDI_DRAW_H_
#define _GDI_DRAW_H_

//#include <windows.h>

class GDIDraw
{
public:
	GDIDraw();
	~GDIDraw();

	bool InitHWND(HWND hWnd);

	bool DrawRGB24(const unsigned char *pBuf, unsigned uWidth, unsigned uHeight);

private:
	HDC m_hdc;
	HWND m_hWnd;
	RECT m_rect;
};

#endif
