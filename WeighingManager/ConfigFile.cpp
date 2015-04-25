/***************************add ConfigFile by zcs 2015.14.25*********/

#include "stdafx.h"
#include "ConfigFile.h"
#include "WeighingManager.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_LENGTH 2500


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CConfigFile::CConfigFile()
{
	m_InitFilePath = "./config.ini";
}

CConfigFile::~CConfigFile()
{

}

void CConfigFile::SetPath(CString strPath)
{
   m_InitFilePath = strPath; 
}

CString CConfigFile::GetPath()
{
   return m_InitFilePath;
}



BOOL CConfigFile::SetKeyValue(CString node, CString key, LPVOID p,UINT size)
{
	BOOL result;
	result=WritePrivateProfileStruct(node,key,p,size,m_InitFilePath);
	return result;
	
}

BOOL CConfigFile::GetKeyValue(CString node, CString key, LPVOID& p, UINT size)
{
	BOOL result=false;
	if(p!=NULL)
	{
		free(p);
		p = NULL;
	}
	p=malloc(size);
	result=GetPrivateProfileStruct(node,key,p,size,m_InitFilePath);
	return result;
}

BOOL CConfigFile::SetStringValue(CString Section, CString strKey, CString strValue)
{
    return WritePrivateProfileString(Section , strKey , strValue , m_InitFilePath);
}


BOOL CConfigFile::GetStringValue(CString node, CString key, CString &value)
{
	BOOL result=TRUE;
	DWORD dw;
	TCHAR strTmp[MAX_LENGTH];
	value="";
	dw=GetPrivateProfileString(node,key,_T(""),strTmp,MAX_LENGTH,m_InitFilePath); 
	if(dw!=0)value=strTmp;
	if (dw==0)result=FALSE;
	return result;
}

CString CConfigFile::GetStringValue(CString Section, CString strKey)
{
    DWORD retval;
    CString strValue;
    TCHAR strTmp[MAX_LENGTH];
    retval = GetPrivateProfileString(Section, strKey, _T("unknown value") , strTmp , MAX_LENGTH , m_InitFilePath);
    if (retval !=0)
    {
        strValue = strTmp;
    }
    else
	{
        strValue = "";
	}
    return strValue;
}

//BOOL CConfigFile::SetPramter(CString dBIp,CString serverIp,CString localIp,CString serverPort,CString localPort,CString g_strServerIp,CString g_DataBaseIp)
//{
//	CString node, key;
//	CString str;
//    CString g_DataSource,g_strConnect;
//
//	GetStringValue((CString)("DB"), (CString)("DataSource"), g_DataSource);
//	g_DataSource.Replace(g_DataBaseIp,dBIp);
//	SetStringValue((CString)("DB"), (CString)("DataSource"), g_DataSource);
//    SetStringValue((CString)("DB"), (CString)("DataSource_IP"), dBIp);
//
//	GetStringValue((CString)("RecordServer"), (CString)("Connect"), g_strConnect);
//	g_strConnect.Replace(g_strServerIp,serverIp);
//	SetStringValue((CString)("RecordServer"), (CString)("Connect"), g_strConnect);
//	SetStringValue((CString)("RecordServer"), (CString)("RecordServer_IP"), serverIp);
//	SetStringValue((CString)("RecordServer"), (CString)("RecordServer_Port"), serverPort);
//
//	SetStringValue((CString)("Local"), (CString)("Local_IP"), localIp);
//	SetStringValue((CString)("Local"), (CString)("LocalNetServer_Port"), localPort);
//  return TRUE;
//}

