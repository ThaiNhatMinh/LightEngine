#pragma once

#include <iostream>
#include <string>
class ISubSystem;
class IContext
{
public:
	virtual ~IContext() = default;
	virtual ISubSystem* GetSystem(const type_info&) = 0;
	virtual void AddSystem(ISubSystem* system) = 0;
};