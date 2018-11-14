#include <pch.h>
#include "OpenGLWindows.h"
#include "OpenGLInput.h"
#include <GLFW/glfw3.h>
#include <tinyxml2/tinyxml2.h>
#include "..\Core\Events.h"
#include "..\Interface\IEventManager.h"
#include "..\Interface\ITimer.h"
#include "..\Interface\ISysUI.h"
#include "Events.h"

namespace Light
{
	void GLFWerrorfun(int code, const char * str)
	{
		E_ERROR("[GLFW]Error code: %d, %s",code, str);
	}

	void OpenGLWindows::ReadConfig()
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile(LIGHT_WINDOW_CONFIG);
		auto pWindows = doc.FirstChildElement("Window");

		m_WindowTitle = pWindows->Attribute("name");

		m_iWidth = pWindows->DoubleAttribute("sizex", 800);
		m_iHeight = pWindows->DoubleAttribute("sizey", 600);
		
	}

	OpenGLWindows::OpenGLWindows(IContext* c)
	{

		ReadConfig();

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		GLFWmonitor* m_pMonitor = glfwGetPrimaryMonitor();

		m_iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		m_iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		m_pWindow = glfwCreateWindow(m_iWidth, m_iHeight, m_WindowTitle.c_str(), nullptr, nullptr);
		if (!m_pWindow)
		{
			E_ERROR("Can't create Window.");
			return;
		}

		glfwSetWindowUserPointer(m_pWindow, c);
		glfwMakeContextCurrent(m_pWindow);
		glfwSwapInterval(0);
		glfwSetCursorPos(m_pWindow, m_iWidth / 2, m_iHeight / 2);
		

		glfwSetMouseButtonCallback(m_pWindow, mouse_button_callback);
		glfwSetCursorPosCallback(m_pWindow, mouse_callback);
		glfwSetScrollCallback(m_pWindow, scroll_callback);
		glfwSetKeyCallback(m_pWindow, key_callback);
		glfwSetCharCallback(m_pWindow, char_callback);

		glfwSetWindowUserPointer(m_pWindow, c);

		glfwSetErrorCallback(GLFWerrorfun);
		SetPos(glm::vec2(-1,-1));

		//HideWindows();
		c->VAddSystem(this);

		/*int	 count;
		const GLFWvidmode* mode = glfwGetVideoModes(m_pMonitor, &count);
		for (int i = 0; i < count; i++)
		{
			cout << mode[i].width << ' ' << mode[i].height << endl;
		}*/

	}


	OpenGLWindows::~OpenGLWindows()
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}

	void OpenGLWindows::SetSize(int W, int H)
	{
		m_iWidth = W;
		m_iHeight = H;
		if (m_pWindow) glfwSetWindowSize(m_pWindow, W, H);
	}

	void OpenGLWindows::SetPos(vec2 pos)
	{
		m_Pos = pos;
		if (pos.x == -1 || pos.y == -1)
		{
			m_Pos.x = m_iScreenWidth / 2 - m_iWidth / 2;
			m_Pos.y = m_iScreenHeight / 2 - m_iHeight / 2;
		}


		if (m_pWindow) glfwSetWindowPos(m_pWindow, m_Pos.x, m_Pos.y);

	}

	void OpenGLWindows::ShowWindows()
	{
		glfwShowWindow(m_pWindow);
	}

	void OpenGLWindows::HideWindows()
	{
		glfwHideWindow(m_pWindow);
	}

	void OpenGLWindows::EnableFullScreen(bool enable)
	{
		if (m_bIsFullscreen) // current full screen
		{
			if (!enable) // set to windowed mode
			{
				int xpos = m_iScreenWidth / 2 - m_iWidth / 2;
				int ypos = m_iScreenHeight / 2 - m_iHeight / 2;
				glfwSetWindowMonitor(m_pWindow, NULL, xpos, ypos, m_iWidth, m_iHeight, GLFW_DONT_CARE);
				m_bIsFullscreen = false;
			}
		}
		else if (!m_bIsFullscreen) // current windowed mode
		{
			if (enable) // set to full screen mode
			{
				GLFWmonitor* m_pMonitor = glfwGetPrimaryMonitor();
				glfwSetWindowMonitor(m_pWindow, m_pMonitor, 0, 0, m_iWidth, m_iHeight, GLFW_DONT_CARE);
				m_bIsFullscreen = true;
			}
		}
	}

	void OpenGLWindows::SwitchMode()
	{
		if (m_bIsFullscreen) EnableFullScreen(false);
		else EnableFullScreen(true);
	}

	int OpenGLWindows::VShouldClose()
	{
		return glfwWindowShouldClose(m_pWindow);
	}

	void OpenGLWindows::SetMouse(int t)
	{
		// Hide OS mouse cursor if ImGui is drawing it

		glfwSetInputMode(m_pWindow, GLFW_CURSOR, t);
	}
	
	vec2 OpenGLWindows::GetMousePos()
	{
		vec2 pos;
		double posx, posy;
		glfwGetCursorPos(m_pWindow, &posx, &posy);
		pos.x = posx;
		pos.y = posy;
		return pos;
	}
	const char * OpenGLWindows::VGetName()
	{
		static const char* pName = typeid(IWindow).name();
		return pName;
	}
	void OpenGLWindows::VGetWindowSize(int & width, int & height)
	{
		width = m_iWidth;
		height = m_iHeight;
	}
	HWND OpenGLWindows::VGetHandle()
	{
		return glfwGetWin32Window(m_pWindow);
	}
	void OpenGLWindows::VSwapBuffer()
	{
		glfwSwapBuffers(m_pWindow);
	}

	void OpenGLWindows::HideMouse(bool isHide)
	{
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, isHide?GLFW_CURSOR_DISABLED: GLFW_CURSOR_NORMAL);
	}

	GLFWwindow * OpenGLWindows::GetGLFW()
	{
		return m_pWindow;
	}


	
	// Is called whenever a key is pressed/released via GLFW
	void OpenGLWindows::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		static IContext* pContext = (IContext*)glfwGetWindowUserPointer(window);
		static OpenGLInput* pInput = static_cast<OpenGLInput*>(pContext->GetSystem<IInput>());
		static IEventManager* pEventManager = pContext->GetSystem<IEventManager>();
		
		
		
		// if unicode key is press
		if (key == -1) return;
		auto pEvent = DEBUG_NEW events::EvtInput();

		pEvent->type = events::Type_KeyChange;
		pEvent->_keyChange.key = (Key)key;
		
		if (action == GLFW_PRESS)
		{
			pInput->keys[key] = true;
			pEvent->_keyChange.action = Press;
		}
		else if (action == GLFW_RELEASE)
		{
			pInput->keys[key] = false;
			pEvent->_keyChange.action = Release;
		}

		pEventManager->VQueueEvent(std::shared_ptr<IEvent>(pEvent));


	}

	static MouseButton toSystemMouse[] =
	{
		Left,       ///< The left mouse button
		Right,      ///< The right mouse button
		Middle,     ///< The middle (wheel) mouse button
		XButton1,   ///< The first extra mouse button
		XButton2,   ///< The second extra mouse button
	};

	void OpenGLWindows::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		static IContext* pContext = (IContext*)glfwGetWindowUserPointer(window);
		static OpenGLInput* pInput = static_cast<OpenGLInput*>(pContext->GetSystem<IInput>());
		static IEventManager* pEventManager = pContext->GetSystem<IEventManager>();

		auto pEvent = DEBUG_NEW events::EvtInput();
		pEvent->type = events::Type_MouseButton;
		pEvent->_mouseButton.button = toSystemMouse[button];

		if (action == GLFW_PRESS)
		{
			pInput->Mouse[button] = true;
			pEvent->_mouseButton.action = Press;
		}
		else if (action == GLFW_RELEASE)
		{
			pEvent->_mouseButton.action = Release;
			
			pInput->Mouse[button] = false;
		}

		pEventManager->VQueueEvent(std::shared_ptr<IEvent>(pEvent));
	}
	void OpenGLWindows::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		static IContext* pContext = (IContext*)glfwGetWindowUserPointer(window);
		static OpenGLInput* pInput = static_cast<OpenGLInput*>(pContext->GetSystem<IInput>());
		static IEventManager* pEventManager = pContext->GetSystem<IEventManager>();

		static bool firstMouse = false;
		static double last_x = 0;
		static double last_y = 0;

		if (!firstMouse)
		{
			last_x = xpos;
			last_y = ypos;
			firstMouse = true;
		}

		float dx = (float)(xpos - last_x);
		float dy = -(float)(ypos - last_y);

		auto pEvent = DEBUG_NEW events::EvtInput();
		pEvent->type = events::Type_MouseMove;
		pEvent->_mouseMove.dx = dx;
		pEvent->_mouseMove.dy = dy;
		pEvent->_mouseMove.x = xpos;
		pEvent->_mouseMove.y = ypos;
		pEventManager->VQueueEvent(std::shared_ptr<IEvent>(pEvent));

		pInput->MouseD[0] = dx;
		pInput->MouseD[1] = dy;
		pInput->m_MousePos[0] = xpos;
		pInput->m_MousePos[1] = ypos;
		last_x = xpos;
		last_y = ypos;

	}
	void OpenGLWindows::scroll_callback(GLFWwindow* window, double x, double y)
	{
		static IContext* pContext = (IContext*)glfwGetWindowUserPointer(window);
		static OpenGLInput* pInput = static_cast<OpenGLInput*>(pContext->GetSystem<IInput>());
		/*static OpenGLSysUI* pSys = static_cast<OpenGLSysUI*>(pContext->GetSystem<ISysUI>());

		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += (float)x;
		io.MouseWheel += (float)y;*/


	}
	void OpenGLWindows::char_callback(GLFWwindow * window, unsigned int c)
	{
		/*ImGuiIO& io = ImGui::GetIO();
		if (c > 0 && c < 0x10000)
			io.AddInputCharacter((unsigned short)c);*/
	}
}