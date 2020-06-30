#pragma once
// http://tmlife.net/programming/c/cpp-singleton-template.html
template<class T>
class SingletonHolder
{
public:
	typedef T Instance;
private:
	static Instance* mInstance;
public:
	static void Create() { if (!mInstance) mInstance = new Instance(); }
	static void Destroy() { if (mInstance) delete mInstance; mInstance = nullptr; }
	static Instance& Ref() { return *mInstance; }
	static Instance* Ptr() { return mInstance; }
private:
	SingletonHolder() {}
	virtual ~SingletonHolder() {}
	SingletonHolder(const SingletonHolder&);
	SingletonHolder& operator=(const SingletonHolder&);
};

template<class T>
typename SingletonHolder<T>::Instance* SingletonHolder<T>::mInstance = nullptr;