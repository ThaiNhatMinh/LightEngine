#pragma once
#include "..\Interface\IWindow.h"

// GLFW
#include <glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>

#include "vec3.hpp"
#include "vec4.hpp"
#include "vec2.hpp"

#include <string>


namespace Light {

	class OpenGLWindows : public IWindow
	{
	private:
		GLuint m_iWidth;
		GLuint m_iHeight;
		glm::vec2 m_Pos;
		GLFWmonitor* m_pMonitor;
		GLFWwindow* m_pWindow;
		std::string m_WindowTitle;
		GLboolean m_bIsFullscreen;
		GLuint m_iScreenWidth;
		GLuint m_iScreenHeight;

	private:
		// Read config from XMLElement
		// return init pos window
		void ReadConfig();

	public:
		OpenGLWindows(IContext * c);
		~OpenGLWindows();

	private:
		void SetSize(int W, int H);
		void SetPos(vec2 pos);
		void ShowWindows();
		void HideWindows();
		void EnableFullScreen(bool enable);
		void SwitchMode();
		int ShouldClose();
		void SetMouse(int t);
		GLFWwindow* Window() { return m_pWindow; };

		glm::vec2 GetMousePos();


		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow*, double, double);
		static void char_callback(GLFWwindow*window, unsigned int c);
	public:
		virtual const char* VGetName();
		virtual void VGetWindowSize(int& width, int& height);
		virtual HWND VGetHandle();
		virtual void VSwapBuffer();
		virtual void HideMouse(bool isHide);

		GLFWwindow*	GetGLFW();
	};

}