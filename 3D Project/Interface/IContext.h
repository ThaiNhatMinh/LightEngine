#pragma once
#include <typeinfo>

#include "ISubSystem.h"

namespace Light
{
	class IContext
	{
	public:
		virtual ~IContext() = default;

		virtual bool VAddSystem(ISubSystem* system) = 0;
		virtual void VMakeContext() = 0;
		virtual void VReleaseContext() = 0;
		template <class T> T* GetSystem();

	protected:
		virtual ISubSystem * VGetSystem(const std::type_info& rtti) = 0;
	};


	template <class T>T* IContext::GetSystem()
	{
		return static_cast<T*>(VGetSystem(typeid(T)));
	}
}