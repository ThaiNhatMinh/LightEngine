#pragma once
#include <pch.h>

// Export C++ function and class to Lua
namespace ScriptExports
{
	void Register(void);
	void Unregister(void);
}