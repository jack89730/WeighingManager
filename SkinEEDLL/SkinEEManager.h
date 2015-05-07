#ifndef _SKINEE_MANAGER_H
#define _SKINEE_MANAGER_H

class CSkinManager
{
protected:
	CSkinManager();
	~CSkinManager();
public:
	static CSkinManager* GetInstance();
	static void          UnLoadSkin();
	LPCTSTR              GetSkinPath();
	void                 SetSkinPath(LPCTSTR lpszSkinPath);
	Image*               GetSkinItem(LPCTSTR lpszFileName);
	Image*               GetResItem(unsigned int nRes);
	void                 LoadSkinFromDLL();
	BOOL                 LoadUIResource(LPCSTR lpszXml);
	
private:
	std::map<LPCTSTR,Image*>  m_mapSkinItems;
	std::map<int,Image*>      m_mapImageItems;
	LPCTSTR                   m_strSkinPath;
	static CSkinManager*      m_Instance;
	ULONG_PTR gdiplusToken;

};

#endif