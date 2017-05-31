#pragma once
#include <iostream>
template <class T>
class Singleton
{
public:
	template<class ...Args>
	static void Init(Args &&...args)
	{
		_instance() = new T(std::forward<Args>(args)...);
	}
	static void Release()
	{
		if (_instance())
		{
			delete _instance();
			_instance() = nullptr;
		}
	}
	static T*	InstancePtr()
	{
		return _instance();
	}
	static T&	Instance()
	{
		return *_instance();
	}

protected:
	Singleton(){}
	virtual ~Singleton() {}
	Singleton(const Singleton&) { }
	Singleton& operator=(const Singleton&) { return *this; }
	static T*& _instance()
	{
		static T* m_pInstance = nullptr;
		return m_pInstance;
	}
private:

	
};