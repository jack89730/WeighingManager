#include "stdafx.h"
#include "DBConnPool.h"
#include "ADOInterface.h"

LRESULT WINAPI InitDB(LPCSTR strConnect,int minConn,int maxConn)
{
	DBConnPool::Instanse()->SetDBInfo(strConnect,minConn,maxConn);
	return DBConnPool::Instanse()->InitializeAllDBConnections();
}

LRESULT WINAPI ExecuteSQL(LPCSTR lpszSQL)//执行sql语句 比如添加记录等
{
	DBConnect *pDBConn = NULL;
	DBConnGuard oConnGuard(pDBConn);
    return pDBConn->ExecuteSQL(lpszSQL);
}

LRESULT WINAPI ExecuteProcSQL(LPCSTR storeProcStr)
{
	DBConnect *pDBConn = NULL;
	DBConnGuard oConnGuard(pDBConn);
	return pDBConn->ExecuteProcSQL(storeProcStr);
}

LRESULT WINAPI QuerySQL(LPCSTR lpszSQL, _RecordsetPtr& pRecordset, CursorTypeEnum cursorType, LockTypeEnum lockType)
{
	DBConnect *pDBConn = NULL;
	DBConnGuard oConnGuard(pDBConn);
	return pDBConn->QuerySQL(lpszSQL, pRecordset, cursorType, lockType);
}

LRESULT WINAPI VarientToString(_variant_t var, LPTSTR& str)
{
	str = (LPTSTR)TCHAR("");
	if (VT_NULL != var.vt)
	{
		str = (LPTSTR)(_bstr_t)var;
	}
	return 0;
}