#ifndef _CONFIGFILE_H__
#define _CONFIGFILE_H__

class CConfigFile 
{
public:
	CConfigFile();
	BOOL    GetKeyValue(CString node, CString key, LPVOID& p, UINT size);
	BOOL    SetKeyValue(CString node, CString key, LPVOID p,UINT size);
    BOOL    SetStringValue(CString Section , CString strKey , CString strValue);
	BOOL    GetStringValue(CString node, CString key, CString &value);
	CString GetStringValue(CString Section, CString strKey);
	

	BOOL    SetPramter(CString dBIp,CString serverIp,CString localIp,CString serverPort,CString localPor,CString g_strServerIp,CString g_DataBaseIp);
	void    SetPath(CString strPath);
	CString GetPath();
    virtual ~CConfigFile();
	
private:
    CString m_InitFilePath;
};

#endif 