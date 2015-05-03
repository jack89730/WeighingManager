#include "stdafx.h"
#include "GDIDraw.h"

static void iDrawToHDC( HDC hDCDst, const RECT* pDstRect, const unsigned char *pSrc, int srcW, int srcH, int bbp);

GDIDraw::GDIDraw()
: m_hdc(NULL), m_hWnd(NULL)
{
}

GDIDraw::~GDIDraw()
{
}

bool GDIDraw::InitHWND(HWND hWnd)
{
	if(hWnd == NULL)
		return(false);

	m_hdc = GetDC(hWnd);

	m_hWnd = hWnd;

	return(m_hdc ? true : false);
}

bool GDIDraw::DrawRGB24(const unsigned char *pBuf, unsigned uWidth, unsigned uHeight)
{
	if(m_hWnd == NULL)
		return(false);

	GetClientRect(m_hWnd, &m_rect);

	iDrawToHDC(m_hdc, &m_rect, pBuf, uWidth, uHeight, 24);

	return(true);
}

static void iFillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin )
{
    BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

    memset( bmih, 0, sizeof(*bmih));
    bmih->biSize = sizeof(BITMAPINFOHEADER);
    bmih->biWidth = width;
    bmih->biHeight = (origin ? abs(height) : -abs(height));
    bmih->biPlanes = 1;
    bmih->biBitCount = (unsigned short)bpp;
    bmih->biCompression = BI_RGB;

    if( bpp == 8 )
    {
        RGBQUAD* palette = bmi->bmiColors;
        int i;
        for( i = 0; i < 256; i++ )
        {
            palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
            palette[i].rgbReserved = 0;
        }
    }
}

static void iDrawToHDC( HDC hDCDst, const RECT* pDstRect, const unsigned char *pSrc, int srcW, int srcH, int bbp) 
{
    if( pDstRect && pSrc)
    {
        unsigned char buffer[sizeof(BITMAPINFOHEADER) + 1024];
        BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		int dstX = pDstRect->left;
		int dstY = pDstRect->top;
		int dstH = pDstRect->bottom - pDstRect->top;
		int dstW = pDstRect->right - pDstRect->left;

        if(srcW > dstW)
        {
            SetStretchBltMode(
                   hDCDst,           // handle to device context
                   HALFTONE );
        }
        else
        {
            SetStretchBltMode(
                   hDCDst,           // handle to device context
                   COLORONCOLOR );
        }

        iFillBitmapInfo( bmi, srcW, srcH, bbp, 0);

        ::StretchDIBits(
            hDCDst,
            dstX, dstY, dstW, dstH,
            0, 0, srcW, srcH,
            pSrc, bmi, DIB_RGB_COLORS, SRCCOPY);
    }
}
