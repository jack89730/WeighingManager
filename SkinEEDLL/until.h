#ifndef SKINEE_UNTIL_H 
#define SKINEE_UNTIL_H 

#ifdef __cplusplus
extern "C" {
#endif

    #define  WM_MYDRAWITEM     WM_USER+1   
    #define  WM_MYCTLCOLOR     WM_USER+2   
    #define  WM_MYMEASUREITEM  WM_USER+3   
    #define  WM_GIFSTOP        WM_USER+3 
    #define  WM_CHANGESKIN     WM_USER+3 

    int GetModulePath(TCHAR* modulePath);
	CString GetProgramPath();

#ifdef __cplusplus
}
#endif

#endif//SKINEE_UNTIL_H