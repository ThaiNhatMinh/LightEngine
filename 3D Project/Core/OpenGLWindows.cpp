#include <pch.h>
#include "OpenGLWindows.h"
#include "OpenGLInput.h"
#include <GLFW/glfw3.h>
#include <tinyxml2/tinyxml2.h>
namespace Light
{

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
		//glfwSetCharCallback(m_pWindow, char_cal);

		glfwSetWindowUserPointer(m_pWindow, c);
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

	int OpenGLWindows::ShouldClose()
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
	void OpenGLWindows::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		static IContext* pContext = (IContext*)glfwGetWindowUserPointer(window);
		static OpenGLInput* pInput = static_cast<OpenGLInput*>(pContext->GetSystem<IInput>());
		
		// if unicode key is press
		if (key == -1) return;

		if (action == GLFW_PRESS)
		{
			pInput->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			pInput->keys[key] = false;
		}
	}

	void OpenGLWindows::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		static IContext* pContext = (IContext*)glfwGetWindowUserPointer(window);
		static OpenGLInput* pInput = static_cast<OpenGLInput*>(pContext->GetSystem<IInput>());
		if (action == GLFW_PRESS)
		{
			pInput->Mouse[button] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			
			pInput->Mouse[button] = false;
		}
	}
	void OpenGLWindows::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		static IContext* pContext = (IContext*)glfwGetWindowUserPointer(window);
		static OpenGLInput* pInput = static_cast<OpenGLInput*>(pContext->GetSystem<IInput>());

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

		pInput->MouseD[0] = dx;
		pInput->MouseD[1] = dy;
		pInput->m_MousePos[0] = xpos;
		pInput->m_MousePos[1] = ypos;
		last_x = xpos;
		last_y = ypos;

	}
	void OpenGLWindows::scroll_callback(GLFWwindow*, double x, double y)
	{
		//cout << x << y << endl;
		
	}
}