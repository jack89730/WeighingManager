#include "stdafx.h"
#include "until.h"

int GetModulePath(TCHAR* strProgramPath)
{
	TCHAR s_buf[MAX_PATH];
	TCHAR s_drive[MAX_PATH];
	TCHAR s_dir[MAX_PATH];

	s_buf[0] = 0x0;
	s_drive[0] = 0x0;
	s_dir[0] = 0x0;

	DWORD n_result = ::GetModuleFileName(NULL, s_buf, sizeof(TCHAR) * MAX_PATH);
	/*errno_t n_err_no =*/ 
	_tsplitpath(s_buf, s_drive, s_dir, NULL, NULL);
	//assert(n_err_no == 0);
	_tcscpy_s(strProgramPath, MAX_PATH,s_drive);
	_tcscat_s(strProgramPath, MAX_PATH,s_dir);

	return 0;
}

CString GetProgramPath()
{
	TCHAR s_buf[MAX_PATH];
	TCHAR s_drive[MAX_PATH];
	TCHAR s_dir[MAX_PATH];

	s_buf[0] = 0x0;
	s_drive[0] = 0x0;
	s_dir[0] = 0x0;

	DWORD n_result = ::GetModuleFileName(NULL, s_buf, sizeof(TCHAR) * MAX_PATH);
	/*errno_t n_err_no =*/ 
	_tsplitpath(s_buf, s_drive, s_dir, NULL, NULL);
	//assert(n_err_no == 0);
	_tcscpy_s(s_buf, MAX_PATH,s_drive);
	_tcscat_s(s_buf, MAX_PATH,s_dir);

	return CString(s_buf);
}