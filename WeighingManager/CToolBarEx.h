#ifndef __TOOLBAREX_H__
#define __TOOLBAREX_H__

class CToolBarEx :public CToolBar
{
public:


protected:
	//{{AFX_MSG(CSXBar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);     //声明消息相应函数
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif