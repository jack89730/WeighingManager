#include "stdafx.h"
#include "include\SkinEE.h"
#include "HookProc.h"
#include "SkinEEManager.h"
#include "until.h"




int __stdcall SkinEE_Attach()  
{  
	CHookProc::getInstance()->g_hWndHook=SetWindowsHookEx(/*WH_CBT*/WH_CALLWNDPROC,CHookProc::HOOKProc,0,GetCurrentThreadId()); 
    
    TCHAR strPath[MAX_PATH];
    GetModulePath(strPath);
    _tcscat_s(strPath, MAX_PATH,_T("skin\\"));
     CSkinManager::GetInstance()->SetSkinPath((LPCTSTR)strPath);

    CString xmlPath =(CString)strPath+_T("def_skin.xml");
    CSkinManager::GetInstance()->LoadUIResource(CW2A(xmlPath));
    return 0;
}  

int __stdcall SkinEE_Detach()  
{  
	UnhookWindowsHookEx(CHookProc::getInstance()->g_hWndHook); 
	return 0;
}

int __stdcall SkinEE_AttachEx(LPCTSTR strSkinFile, LPCTSTR strPassword)
{ 
	CString m_strSkinPath=(CString)strSkinFile;
	m_strSkinPath = m_strSkinPath.Left(m_strSkinPath.ReverseFind(_T('\\'))+1);
	
	CSkinManager::UnLoadSkin();
	CSkinManager::GetInstance()->SetSkinPath(m_strSkinPath);
	CSkinManager::GetInstance()->LoadUIResource(CW2A(strSkinFile));
 
	for(map<HWND,WNDPROC>::iterator it=CHookProc::getInstance()->m_OldFunc.begin();it!=CHookProc::getInstance()->m_OldFunc.end();it++)
	{
      SendMessage(it->first,WM_CHANGESKIN,NULL,NULL);
	}

    return 0;
}
