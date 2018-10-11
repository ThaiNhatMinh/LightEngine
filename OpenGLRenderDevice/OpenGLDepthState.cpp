#include "stdafx.h"
#include "OpenGLDepthState.h"
#include "DataMap.h"
Light::render::OpenGLDepthState::OpenGLDepthState(bool enable, bool mask, Compare depthFunc)
{
	
	DepthEnable =enable;
	DepthMask = mask ? GL_TRUE : GL_FALSE;
	Depthfunc = DepthFunc[depthFunc];
}
