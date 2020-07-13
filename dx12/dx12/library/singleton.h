#pragma once
// http://tmlife.net/programming/c/cpp-singleton-template.html
template<class T>
class SingletonHolder
{
public:
	typedef T Instance;
private:
	static Instance* m_pInstance;
public:
	static void Create() { if (!m_pInstance) m_pInstance = new Instance(); }
	static void Destroy() { if (m_pInstance) delete m_pInstance; m_pInstance = nullptr; }
	static Instance& Ref() { return *m_pInstance; }
	static Instance* Ptr() { return m_pInstance; }
private:
	SingletonHolder() {}
	virtual ~SingletonHolder() {}
	SingletonHolder(const SingletonHolder&);
	SingletonHolder& operator=(const SingletonHolder&);
};

template<class T>
typename SingletonHolder<T>::Instance* SingletonHolder<T>::m_pInstance = nullptr;