#ifndef __GLOBALOBJECT_H__
#define __GLOBALOBJECT_H__

template<class T>
class Singleton
{
public:
	static T* GetInstance()
	{
		if (NULL == m_pInstance)
		{
			m_pInstance = new T();
			atexit(Destory);
		}
		return m_pInstance;
	}

protected:
	Singleton(){};

private:
	Singleton(const Singleton &);
	Singleton& operator=(const Singleton &);

	static void Destory()
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}

private:
	static  T* volatile m_pInstance;
};

template <typename T>
T* volatile Singleton<T>::m_pInstance = NULL;


class CGlobalObject :public Singleton<CGlobalObject>
{
private:
	CGlobalObject() :m_strUserNmae(""),
		m_strCurrentTime("")
	{

	}
public:
	CString GetUsername()
	{
		return m_strUserNmae;
	}

	CString GetCurrentTime()
	{
		return m_strCurrentTime;
	}

	void SetUsername(CString strUserNmae)
	{
		m_strUserNmae = strUserNmae;
	}

	void SetCurrentTime(CString strCurrentTime)
	{
		m_strCurrentTime = strCurrentTime;
	}
private:
	CString m_strUserNmae;
	CString m_strCurrentTime;
	friend Singleton<CGlobalObject>;
};
#endif