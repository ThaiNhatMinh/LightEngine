#pragma once
#include <iostream>
template <class T>
class Singleton
{
public:
	template<class ...Args>
	static void startUp(Args &&...args)
	{
		_instance() = new T(std::forward<Args>(args)...);
		_instance()->onStartUp();
	}
	static void shutDown()
	{
		if (_instance())
		{
			_instance()->onShutDown();
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
	
	//Override if you want your module to be notified once it has been constructed and started.
	virtual void onStartUp() {}

	
	//Override if you want your module to be notified just before it is deleted.
	virtual void onShutDown() {}

private:

	
};