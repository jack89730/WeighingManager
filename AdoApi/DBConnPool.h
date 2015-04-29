#ifndef DBCONNPOOL_H
#define DBCONNPOOL_H

#include "stdafx.h"
#include <list>
#include <string>

#define  SQL_BUFFER_LEN 1024
#define  COMMAND_TIMEOUT 2000
#define  CONNECT_TIMEOUT 2000

class  DBConnect
{
public:
	 DBConnect(LPCSTR strConnect,BOOL &state);
	~DBConnect();
public:
	// 连接到数据库
	int Open(LPCSTR strConnect);
	// 关闭数据库
	int Close();
	// 数据库是否已连接
	BOOL IsOpen() const;

private:
	ADODB::_ConnectionPtr     m_pAdoConn;   //ADO的数据库连接智能指针
	ADODB::_CommandPtr        m_pCommand;
	bool                      _isAdoValid;       //ADO环境是否已经初化成功标志量
	bool                      m_bDBOpen;

	LPCSTR       _strDstAddress;    //数据源地址或服务名
	LPCSTR       _strUsername;      //数据库用户名
	LPCSTR       _strPassword;      //数据库密码
	LPCSTR       _strDBName;        //数据库名称
	//void         VarientToString(_variant_t var, string& str);
	//对外公共接口
public:
	LRESULT	   ExecuteSQL(LPCSTR lpszSQL);//执行sql语句 比如添加记录等
	LRESULT    ExecuteProcSQL(LPCSTR storeProcStr);
	LRESULT    QuerySQL(LPCSTR lpszSQL, _RecordsetPtr& pRecordset, CursorTypeEnum cursorType = adOpenForwardOnly, LockTypeEnum lockType = adLockReadOnly);
	LRESULT    UserLogin(LPCSTR name,LPCSTR password);
}; 
typedef std::list<DBConnect*> DBConnectList;

class  DBConnPool  
{
public:
	DBConnPool();
	virtual ~DBConnPool();
	// 获取实例指针
	static DBConnPool* Instanse();
	// 初始化所有连接
	int InitializeAllDBConnections();

	// 关闭所有连接
	void DestroyAllDBConnections();
	// 获取一个空闲连接
	DBConnect* GetAConnection();
	// 交还连接给空闲队列
	int RestoreAConnection(DBConnect* pDBEngine);
	void SetDBInfo(LPCSTR strConnect,int minConn,int maxConn);
private:
 
	// 创建一个连接
	int InitializeAConnection();
	// 关闭一个连接
	void CloseAConnection(DBConnect* pDBEngine);
	// 停止工作线程
	void StopThread();

	// 判断是否需要停止
	BOOL IsNeedStop();
	BOOL IsNeedConnection();
	// 将守卫类作为连接池类的友元类
	friend class ConnGuard;
	// 唯一实例
	static DBConnPool *m_pInstanse;

	// 空闲数据库连接队列
	DBConnectList m_listIdleConnection;
	// 在使用的数据库连接
	DBConnectList m_listBusyConnection;

	// 队列保护的临界区
	CRITICAL_SECTION m_csIdleConnList;
	CRITICAL_SECTION m_csBusyConnList;

	// 可用连接总数的三个指标：最大、最小
	unsigned int m_nMaxCount;
	unsigned int m_nMinCount;

	// 数据库信息
	LPCSTR       _strDstAddress;    //数据源地址或服务名
	LPCSTR       _strUsername;      //数据库用户名
	LPCSTR       _strPassword;      //数据库密码
	LPCSTR       _strDBName;        //数据库名称
	LPCSTR       _strConnect;       //链接字符串

	// 维护线程
	HANDLE m_hMaintanceThread; // 线程句柄
	HANDLE m_hHaveData; // 信号

	BOOL m_bNeedStop; // 管理线程起停的标志位
	BOOL m_bNeedConnection; // 需要创建连接的标志
	static DWORD WINAPI thread_run( LPVOID pdata);
};
// 守卫类，利用构造和析构函数保证连接取出和归还必须成对，防止资源泄露
class  DBConnGuard
{
public:
	DBConnGuard(DBConnect*& DBConn)
	{
		DBConn = DBConnPool::Instanse()->GetAConnection();
		m_pDBConn = DBConn;
	}
	virtual ~DBConnGuard()
	{
		DBConnPool::Instanse()->RestoreAConnection(m_pDBConn);
	}
private:
	DBConnect *m_pDBConn;
};
#endif // !defined(AFX_DBCONNPOOL_H__42089E9A_FD85_4DD4_A973_64A4980332A5__INCLUDED_)
