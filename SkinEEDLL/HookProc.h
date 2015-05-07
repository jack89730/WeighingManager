
#ifndef _SKINEE_HOOKPROC_H
#define _SKINEE_HOOKPROC_H

typedef LRESULT (*WNDPROC1)(HWND);

class CHookProc
{
protected:
	CHookProc();
	~CHookProc();

public:
	template <typename T>
	static int GetControlType(T key)
	{
		map<T,int>::iterator iter;
		iter = m_Instance->m_mapControlItems.find(key);
		if (iter ==  m_Instance->m_mapControlItems.end())
		{
			return 0;
		}
		else
		{
			return  iter->second;
		}
	}
	void   SetHOOKProcMap();

public:
	static CHookProc* getInstance();
	static LRESULT CALLBACK HOOKProc(int code, WPARAM wParam, LPARAM lParam);  
    static LRESULT  HOOKProcDialog(HWND hWnd);
	static LRESULT  HOOKProcButton(HWND hWnd);
	static LRESULT  HOOKProcEdit(HWND hWnd);
    static LRESULT  HOOKProcListBox(HWND hWnd);
	static LRESULT  HOOKProcStatic(HWND hWnd);
    
    static LRESULT CALLBACK ListBoxWndProc(HWND hwnd, UINT uMsg, WPARAM wParam,  LPARAM lParam);
    //static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT uMsg, WPARAM wParam,  LPARAM lParam);

	static LRESULT CALLBACK HOOKProc_CBT(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT HOOKProcDialog_CBT(HWND hWnd);
	static LRESULT CALLBACK DialogWndProc_CBT(HWND hwnd, UINT uMsg, WPARAM wParam,  LPARAM lParam);
	
	 

protected:
	static CHookProc*  m_Instance;
	
public:
	HHOOK              g_hWndHook;
	HINSTANCE          g_hMod;
	map<HWND,WNDPROC>  m_OldFunc;
	map<int,WNDPROC1>  m_FuncMap;
    map<CString,int>   m_mapControlItems;

};

/*HOOKPROC静态成员初始化*/
__declspec(selectany) CHookProc *CHookProc::m_Instance = NULL;

#endif /*_SKINEE_HOOKPROC_H*/