#include "StdAfx.h"
#include "SkinEEManager.h"
#include "import\tinyxml2.h"
//#include "until.h"


CSkinManager* CSkinManager::m_Instance = NULL;

CSkinManager::CSkinManager():m_strSkinPath(NULL)
{
	GdiplusStartupInput  gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

CSkinManager::~CSkinManager()
{
	for (map<LPCTSTR,Image*>::iterator iter = m_mapSkinItems.begin();iter != m_mapSkinItems.end();iter++)
	{
		delete iter->second;
	}

	for ( map<int,Image*>::iterator iter = m_mapImageItems.begin();iter != m_mapImageItems.end();iter++)
	{
		delete iter->second;
	}
	GdiplusShutdown(gdiplusToken);
}

CSkinManager* CSkinManager::GetInstance()
{
	if (m_Instance==NULL)
	{
		m_Instance = new CSkinManager;
	}

	return m_Instance;
}

void  CSkinManager::SetSkinPath(LPCTSTR lpszSkinPath)
{
	m_strSkinPath = lpszSkinPath;
}

LPCTSTR CSkinManager::GetSkinPath()
{
	return m_strSkinPath;
}

Image* CSkinManager::GetSkinItem(LPCTSTR lpszFileName)
{
	Image* pImage = NULL;
	map<LPCTSTR,Image*>::iterator iter;
	iter = m_mapSkinItems.find(lpszFileName);
	if (iter == m_mapSkinItems.end())
	{
		TCHAR * strImgPath= new TCHAR[_tcslen(m_strSkinPath)+_tcslen(lpszFileName)+1];
		_tcscpy(strImgPath, m_strSkinPath);
		_tcscat(strImgPath,lpszFileName); 
       // CString strImgPath = (CString)m_strSkinPath+(CString)lpszFileName;
		m_mapSkinItems[lpszFileName] = (Image*)Bitmap::FromFile(strImgPath);
		pImage = m_mapSkinItems[lpszFileName];
	}
	else
	{
		pImage = iter->second;
	}

	return pImage;
}

Image* CSkinManager::GetResItem(int nRes)
{
	if (nRes>m_mapImageItems.size()||nRes<1)
		return NULL;

	return m_mapImageItems[nRes];
}

BOOL CSkinManager::LoadUIResource(LPCSTR lpszXml)
{
	LPCSTR strTypeName;
	LPCSTR strResID;
	LPCSTR strImgPath;
	tinyxml2::XMLDocument xmlDoc;
	XMLElement* pXmlRootElem;
	
	xmlDoc.LoadFile(lpszXml); 
	if (xmlDoc.Error())
		return FALSE;

	pXmlRootElem=xmlDoc.RootElement();
	if (!pXmlRootElem)
		return FALSE;
	if (strcmp(pXmlRootElem->Value(), "skins") != 0)
	    return FALSE;
	
	for (XMLElement* pXmlChild = pXmlRootElem->FirstChildElement(); NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
	{
		strTypeName = pXmlChild->Value();
		strResID = pXmlChild->Attribute("id");
		strImgPath = pXmlChild->Attribute("src");
		
		if (!strTypeName || !strResID||!strImgPath)
			continue;

        if(m_mapImageItems[atoi(strResID)]!=NULL)
           delete m_mapImageItems[atoi(strResID)];
     
		USES_CONVERSION;  //T2CW()UNIcode与多字节转化
		CString str=(CString)m_strSkinPath+(CString)A2CW(strImgPath);
		m_mapImageItems[atoi(strResID)]=(Image*)Bitmap::FromFile(str/*A2CW(strImgPath)*/);
	}	

	return TRUE;
}

void  CSkinManager::UnLoadSkin()
{
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}
}