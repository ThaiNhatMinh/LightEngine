#include <pch.h>
#include "..\Interface\IWindow.h"
#include "Context.h"
#include "OpenGLWindows.h"
namespace Light {
	Context::Context()
	{

	}

	Context::~Context()
	{
	}


	bool Context::VAddSystem(ISubSystem * system)
	{
		for (auto& el : m_Systems)
			if (typeid(*el) == typeid(*system)) return false;

		m_Systems.push_back(system);
		return true;
	}

	void Context::VMakeContext()
	{
		
	}

	ISubSystem * Context::VGetSystem(const std::type_info & rtti)
	{
		for (auto& el : m_Systems)
		{
#if	defined(DEBUG) || defined(_DEBUG)
			
			cout <<"---" << typeid(*el).name() <<"__" << typeid(*el).hash_code() << endl;
#endif
			if (!strcmp(el->VGetName(), rtti.name())) return el;
		}
		
		return nullptr;
	}

}