#pragma once

#include <Windows.h>

namespace Light
{

	static const char* LIGHT_WINDOW_CONFIG = "Configs//Window.xml";

	class IWindow : public ISubSystem
	{
	public:

		virtual void	VGetWindowSize(int& width, int& height) = 0;
		virtual void	HideMouse(bool isHide) = 0;
		virtual HWND	VGetHandle() = 0;
		virtual void	VSwapBuffer() = 0;
		virtual int		VShouldClose() = 0;
	};
}