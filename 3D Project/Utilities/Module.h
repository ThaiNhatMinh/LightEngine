#pragma once
#include <iostream>

// Singleton for system
class Context;
class Singleton
{
protected:
	Singleton(Context* c){}
	virtual ~Singleton() {}
	Singleton(const Singleton&) {}
	Singleton& operator=(const Singleton&) { return *this; }
	
};