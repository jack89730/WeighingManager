
#include "stdafx.h"
#include "DBConnPool.h"
#include <tchar.h>


//////////////////////////////////////DBConnect/////////////////////////
DBConnect::DBConnect(LPCSTR strConnect,BOOL &state)
{
	m_bDBOpen = false;
	if (0 == Open(strConnect))
	{
		state =true;
	}
	else
	{
		state = false;
	}
}
DBConnect::~DBConnect()
{
	Close();
}
int DBConnect::Open(LPCSTR strConnect)
{
	HRESULT hr = ::CoInitialize(NULL);
	if (FAILED(hr))
	{
		return -1;
	}

	hr = m_pAdoConn.CreateInstance(__uuidof(Connection));
	if (FAILED(hr))
	{
		return -1;
	}
	/*m_pAdoConn->ConnectionTimeout = COMMAND_TIMEOUT;
	m_pAdoConn->CommandTimeout = CONNECT_TIMEOUT;*/

	try
	{
		// 连接到服务器上数据库
		hr =m_pAdoConn->Open((bstr_t)strConnect, "", "",adModeUnknown) ;
		if (FAILED(hr))
			return -1;
	}
	catch (_com_error e)
	{
		MessageBox(NULL,e.Description(),_T("提示") ,1);
		//throw e;
		//OutputDebugString(_T("数据库操作失败"));
		/*char *szlog=NULL;
		sprintf(szlog, "数据库操作失败! 错误信息:%s, 文件:%s, 行:%d./n", e.Description(), __FILE__, __LINE__);*/
		//TRACE(_T("数据库操作失败! 错误信息:%s, 文件:%s, 行:%d./n"), err.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}
	m_bDBOpen = TRUE;
	return 0;
}
int DBConnect::Close()
{
	if (m_bDBOpen)
	{
		HRESULT hr =m_pAdoConn->Close();

		if (FAILED(hr))
		{
			return -1;
		}
		::CoUninitialize();
		m_bDBOpen = FALSE;
	}

	return 0;
}

LRESULT  DBConnect::QuerySQL(LPCSTR bstrSQL, _RecordsetPtr& pRecordset, CursorTypeEnum cursorType, LockTypeEnum lockType)
{
	if (!m_bDBOpen||m_pAdoConn->GetState()!=ADODB::adStateOpen)
	{
		return -1;
	}

	try
	{
		pRecordset.CreateInstance(__uuidof(Recordset));
		pRecordset->Open((_bstr_t)bstrSQL, m_pAdoConn.GetInterfacePtr(), cursorType /*adOpenStatic*/ /*adOpenDynamic*/, lockType/*adLockOptimistic*/, adCmdText);
	}
	catch(_com_error e)
	{
		MessageBox(NULL,e.Description(),_T("提示") ,1);
		throw e;
	}
	return 0;
}

LRESULT  DBConnect::ExecuteProcSQL(LPCSTR storeProcStr)
{
	if (!m_bDBOpen||m_pAdoConn->GetState()!=ADODB::adStateOpen)
	{
		return -1;
	}

	HRESULT hr = m_pCommand.CreateInstance(__uuidof(Command));

	try
	{
		m_pCommand.CreateInstance("ADODB.Command");
		m_pCommand->ActiveConnection = m_pAdoConn;

		m_pCommand->CommandText = (_bstr_t)storeProcStr;//存储过程为Test
		m_pCommand->CommandType = adCmdStoredProc;
	}
	catch(_com_error e)
	{
		MessageBox(NULL,e.Description(),_T("提示") ,1);
		throw e;
	}
	return 0;//成功
}

LRESULT  DBConnect::ExecuteSQL(LPCSTR lpszSQL)
{
	if (!m_bDBOpen||m_pAdoConn->GetState()!=ADODB::adStateOpen)
	{
		return -1;
	}

	_bstr_t bstrSQL = (_bstr_t)lpszSQL;
	_variant_t RecordsAffected;//返回影响的记录条数 现在这应该是返回1条

	try
	{
		m_pAdoConn->Execute(bstrSQL,&RecordsAffected,(LONG)-1);
	}
	catch(_com_error e)
	{
		MessageBox(NULL,e.Description(),_T("提示") ,1);
		throw e;
	}
	return (long)RecordsAffected;
}

//////////////////////////////////////End//////////////////////////////
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DBConnPool* DBConnPool::m_pInstanse = NULL;
DBConnPool::DBConnPool()
{
	m_bNeedStop = FALSE;
	m_bNeedConnection = FALSE;
	m_hMaintanceThread = INVALID_HANDLE_VALUE;

	// 线程控制
	m_hHaveData = CreateEvent (NULL, TRUE, FALSE, _T("DataConnPool"));


	InitializeCriticalSection(&m_csIdleConnList);
	InitializeCriticalSection(&m_csBusyConnList);
}

void DBConnPool::SetDBInfo(LPCSTR strConnect ,int minConn, int maxConn)
{
	_strConnect=strConnect;
	m_nMaxCount = maxConn;
	m_nMinCount = minConn;
}

DBConnPool::~DBConnPool()
{
	m_hMaintanceThread = INVALID_HANDLE_VALUE;
	m_bNeedStop = TRUE;
	CloseHandle(m_hHaveData);
	CloseHandle(m_hMaintanceThread);

	DeleteCriticalSection(&m_csIdleConnList);
	DeleteCriticalSection(&m_csBusyConnList);
}

DBConnPool *DBConnPool::Instanse()
{
	if (NULL == m_pInstanse)
	{
		m_pInstanse = new DBConnPool();
	}
	return m_pInstanse;
}

int DBConnPool::InitializeAllDBConnections()
{
	// 先七公里现有的数据
	DestroyAllDBConnections();
	// 开始按照最小数量开始创建
	DBConnect* pDBConnect = NULL;
	int nCount = 0;
	for (unsigned int i = 0; i < m_nMinCount; i++)
	{
		nCount = InitializeAConnection();
	}
	// 创建一个工作线程，用来进行一些后台维护工作
	if (INVALID_HANDLE_VALUE == m_hMaintanceThread)
	{
		m_hMaintanceThread = CreateThread(NULL, NULL, thread_run, (LPVOID)this, 0, NULL);
	}
	return nCount;
}

void DBConnPool::DestroyAllDBConnections()
{
	// 销毁数据库连接可以使用大锁
	EnterCriticalSection(&m_csIdleConnList);
	DBConnectList::iterator itIdle = m_listIdleConnection.begin();
	DBConnectList::iterator itIdleEnd = m_listIdleConnection.end();
	while (itIdle != itIdleEnd)
	{
		if (NULL != (*itIdle))
		{
			(*itIdle)->Close();
			delete (*itIdle);
		}
		// erase本身就会把跌代器指向下一个
		itIdle = m_listIdleConnection.erase(itIdle);
	}
	LeaveCriticalSection(&m_csIdleConnList);
	// 还有使用中的连接
	EnterCriticalSection(&m_csBusyConnList);
	DBConnectList::iterator itBusy = m_listBusyConnection.begin();
	DBConnectList::iterator itBusyEnd = m_listBusyConnection.end();
	while (itBusy != itBusyEnd)
	{
		if (NULL != (*itBusy))
		{
			(*itBusy)->Close();
			delete (*itBusy);
		}
		// erase本身就会把跌代器指向下一个
		itBusy = m_listBusyConnection.erase(itBusy);
	}
	LeaveCriticalSection(&m_csBusyConnList);
}

int DBConnPool::InitializeAConnection()
{
	BOOL bSuccess = FALSE;
	DBConnect* pDBEngine = new DBConnect(_strConnect, bSuccess);
	if (bSuccess)
	{
		m_bNeedConnection = FALSE;
		return RestoreAConnection(pDBEngine);
	}
	else
	{
		delete pDBEngine;
		return m_listIdleConnection.size();
	}
}

void DBConnPool::CloseAConnection(DBConnect* pDBEngine)
{
	pDBEngine->Close();
	// 从空闲队列将其删除
	EnterCriticalSection(&m_csIdleConnList);
	m_listIdleConnection.remove(pDBEngine);
	LeaveCriticalSection(&m_csIdleConnList);
}

DBConnect* DBConnPool::GetAConnection()
{
	DBConnect * pDBEngine = NULL;
	// 做一个循环，反复尝试五次取连接，每次间隔1秒钟
	int nTimes = 0;
	while ((m_listIdleConnection.size() <= 0) && (nTimes < 5))
	{
		Sleep(1000);
		nTimes++;
	}
	if (5 == nTimes)
	{
		// 这样狼狈的进来肯定是没有可用连接了，记录日志退出
		// g_pSvrLog->AddRunLog(LL_ERROR, _T("Waiting for a connection for a long time, but failed."));
		return pDBEngine;
	}
	// 从空闲队列中取出，并且加入到使用队列中
	EnterCriticalSection(&m_csIdleConnList);
	if (m_listIdleConnection.size() > 0)
	{
		pDBEngine = m_listIdleConnection.front();
		m_listIdleConnection.pop_front();
		// 加入使用的连接队列
		EnterCriticalSection(&m_csBusyConnList);
		m_listBusyConnection.push_back(pDBEngine);
		LeaveCriticalSection(&m_csBusyConnList);
	}
	LeaveCriticalSection(&m_csIdleConnList);
	if (m_listIdleConnection.size() <= 1)
	{
		// 剩余空闲连接的数目小于等于1个时候需要检查开始创建
		if ((m_listIdleConnection.size() + m_listBusyConnection.size()) < m_nMaxCount)
		{
			// 还小于最大限制，可以创建
			SetEvent(m_hHaveData);
			m_bNeedConnection = TRUE;
		}
		else
		{
			// 超出限制了，做个记录吧
			//g_pSvrLog->AddRunLog(LL_ERROR, _T("Database connection reached max count."));
		}
	}
	return pDBEngine;
}

int DBConnPool::RestoreAConnection(DBConnect* pDBEngine)
{
	if (NULL != pDBEngine)
	{
		// 从使用中的队列取出
		EnterCriticalSection(&m_csBusyConnList);
		m_listBusyConnection.remove(pDBEngine);
		LeaveCriticalSection(&m_csBusyConnList);
		// 加入到空闲队列中
		EnterCriticalSection(&m_csIdleConnList);
		m_listIdleConnection.push_back(pDBEngine);
		LeaveCriticalSection(&m_csIdleConnList);
	}
	EnterCriticalSection(&m_csIdleConnList);
	int nCount = m_listIdleConnection.size();
	LeaveCriticalSection(&m_csIdleConnList);
	return nCount;
}

void DBConnPool::StopThread()
{
	m_bNeedStop = TRUE;
	// 因为线程是无限制等待信号的，所以这里先把标志位置为停止，然后发信号让线程检测
	SetEvent(m_hHaveData);
	// 等待退出
	WaitForSingleObject(m_hMaintanceThread, INFINITE);
	CloseHandle(m_hMaintanceThread);
}

BOOL DBConnPool::IsNeedStop()
{
	return m_bNeedStop;
}

BOOL DBConnPool::IsNeedConnection()
{
	return m_bNeedConnection;
}
/************************************************************************/
/* 维护线程                                                             */
/************************************************************************/
DWORD WINAPI DBConnPool::thread_run( LPVOID pdata)
{
	DBConnPool * pConPool = (DBConnPool *) pdata;
	while (!pConPool->IsNeedStop())
	{
		// 设置事件为无信号, 并且无限制等待
		ResetEvent(pConPool->m_hHaveData);  
		WaitForSingleObject(pConPool->m_hHaveData, INFINITE);
		if (pConPool->IsNeedConnection())
		{
			// g_pSvrLog->AddRunLog(LL_DEBUG, _T("Create a new DB connection."));
			pConPool->InitializeAConnection();
		}
	}
	return 0;
}
