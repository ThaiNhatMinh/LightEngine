#include "pch.h"



Windows::Windows(string title,int W,int H)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWmonitor* m_pMonitor = glfwGetPrimaryMonitor();

	m_iWidth = W;
	m_iHeight = H;
	m_WindowTitle = title;

	m_iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	m_iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	/*int	 count;
	const GLFWvidmode* mode = glfwGetVideoModes(m_pMonitor, &count);
	for (int i = 0; i < count; i++)
	{
		cout << mode[i].width << ' ' << mode[i].height << endl;
	}*/

}


Windows::~Windows()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

bool Windows::InitWindow()
{
	
	GLFWwindow* window = glfwCreateWindow(m_iWidth, m_iHeight, m_WindowTitle.c_str(), nullptr, nullptr);
	if (!window)
	{
		Log::Message(Log::LOG_ERROR, "Can't create Window.");
		return false;
	}
	
	
	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);
	glfwSetCursorPos(window, m_iWidth / 2, m_iHeight / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = GL_TRUE;
	
	
	//glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	m_pWindow = window;

	HideWindows();
	
	return true;
}

void Windows::SetSize(int W, int H)
{
	m_iWidth = W;
	m_iHeight = H;
	if (m_pWindow) glfwSetWindowSize(m_pWindow, W, H);
}

void Windows::SetPos(vec2 pos)
{
	m_Pos = pos;
	if (pos.x == -1 || pos.y == -1)
	{
		m_Pos.x = m_iScreenWidth / 2 - m_iWidth / 2;
		m_Pos.y = m_iScreenHeight / 2 - m_iHeight / 2;
	}


	if(m_pWindow) glfwSetWindowPos(m_pWindow, m_Pos.x, m_Pos.y);
	
}

void Windows::ShowWindows()
{
	glfwShowWindow(m_pWindow);
}

void Windows::HideWindows()
{
	glfwHideWindow(m_pWindow);
}

void Windows::EnableFullScreen(bool enable)
{
	if (m_bIsFullscreen) // current full screen
	{
		if (!enable) // set to windowed mode
		{
			int xpos = m_iScreenWidth / 2 - m_iWidth / 2;
			int ypos = m_iScreenHeight / 2 - m_iHeight / 2;
			glfwSetWindowMonitor(m_pWindow, NULL, xpos , ypos, m_iWidth, m_iHeight, GLFW_DONT_CARE);
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

void Windows::SwitchMode()
{
	if (m_bIsFullscreen) EnableFullScreen(false);
	else EnableFullScreen(true);
}
