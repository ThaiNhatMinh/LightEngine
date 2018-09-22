#include "pch.h"
#include "OpenGLInput.h"

#include <glfw3.h>

#include "..\Graphics3D\OpenGL\DataMap.h"

namespace Light
{
	

	OpenGLInput::OpenGLInput(IContext * pContext)
	{
		memset(keys, 0, sizeof(keys));
		pContext->VAddSystem(this);
	}

	void OpenGLInput::VUpdate()
	{
		MouseD[0] = MouseD[1] = 0;
		glfwPollEvents();
	}

	const char * OpenGLInput::VGetName()
	{
		static const char* pname = typeid(IInput).name();
		return pname;
	}

	bool OpenGLInput::VOnKey(Key k)
	{
		return keys[toglfw3Key[k]];
	}

	bool OpenGLInput::VOnMouse(MouseButton m)
	{
		return Mouse[toglfw3Mouse[m]];
	}

	void OpenGLInput::VMouseDeltaPos(int & dx, int & dy)
	{
		
		dx = MouseD[0];
		dy = MouseD[1];
		
	}

}