#pragma once


class CDrawSlider
{
public:
    CDrawSlider();
    virtual ~CDrawSlider();

protected:
    BOOL OnSliderDrawThumb( CDC* pDC,  CRect& rect, UINT nState);
    BOOL OnSliderDrawChannel( CDC* pDC,  CRect& rect, UINT nState);
    BOOL OnSliderDrawTics( CDC* pDC,  CRect& rect, UINT nState){ return TRUE;};

private:
    CBitmap m_bmpDrag;
    CBitmap m_bmpBg;
public:
    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
protected:
    virtual void PreSubclassWindow();
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    LRESULT OnNotifyTransparent( WPARAM wParam, LPARAM lParam);
};