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
		template <class T> T* GetSystem();

	protected:
		virtual ISubSystem * VGetSystem(const std::type_info& rtti) = 0;
	};


	template <class T>T* IContext::GetSystem()
	{
#if	defined(DEBUG) || defined(_DEBUG)
		std::cout << typeid(T*).name() << std::endl;
#endif
		return static_cast<T*>(VGetSystem(typeid(T)));
	}
}