#include <pch.h>
#include "..\Interface\IWindow.h"
#include "Context.h"
#include "OpenGLWindows.h"
namespace Light {
	Context::Context():pthread(nullptr)
	{

		
		
	}

	Context::~Context()
	{
		if (pthread) glfwDestroyWindow((GLFWwindow*)pthread);
	}


	bool Context::VAddSystem(ISubSystem * system)
	{
		for (auto& el : m_Systems)
			if (typeid(*el) == typeid(*system)) return false;

		m_Systems.push_back(system);

		if (typeid(*system) == typeid(OpenGLWindows))
		{
			OpenGLWindows* pWindow = static_cast<OpenGLWindows*>(system);

			auto p = pWindow->GetGLFW();
			pthread = glfwCreateWindow(2, 2, "Load", nullptr, p);
			if (pthread == nullptr)
			{
				E_ERROR("Failed to create thread loading...");
			}
			glfwHideWindow((GLFWwindow*)pthread);
		}
		return true;
	}

	void Context::VMakeContext()
	{
		m_Lock.lock();
		OpenGLWindows* pWindow = static_cast<OpenGLWindows*>(GetSystem<IWindow>());
		auto p = pWindow->GetGLFW();
		GLFWwindow* window = glfwGetCurrentContext();
		glfwMakeContextCurrent((GLFWwindow*)pthread);
		window = glfwGetCurrentContext();
		m_Lock.unlock();
	}

	void Context::VReleaseContext()
	{
		
		/*OpenGLWindows* pWindow = static_cast<OpenGLWindows*>(GetSystem<IWindow>());
		GLFWwindow* window = glfwGetCurrentContext();
		glfwMakeContextCurrent(window);*/
	}

	ISubSystem * Context::VGetSystem(const std::type_info & rtti)
	{
		for (auto& el : m_Systems)
		{
			if (!strcmp(el->VGetName(), rtti.name())) return el;
		}
		
		return nullptr;
	}

}