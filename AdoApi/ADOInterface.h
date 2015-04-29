#ifndef ADOINTERFACE_H
#define ADOINTERFACE_H
#include "stdafx.h"
#if defined(__cplusplus)
extern "C"
{
#endif

	LRESULT   WINAPI  InitDB(LPCSTR strConnect,int minConn = 1,int maxConn = 10);
	LRESULT	  WINAPI  ExecuteSQL(LPCSTR lpszSQL);//执行sql语句 比如添加记录等
	LRESULT   WINAPI  ExecuteProcSQL(LPCSTR storeProcStr);
	LRESULT   WINAPI  QuerySQL(LPCSTR lpszSQL, _RecordsetPtr& pRecordset, CursorTypeEnum cursorType = adOpenForwardOnly, LockTypeEnum lockType = adLockReadOnly);
	LRESULT   WINAPI  VarientToString(_variant_t var, LPTSTR& str);

#if defined(__cplusplus)
}
#endif
#endif 
