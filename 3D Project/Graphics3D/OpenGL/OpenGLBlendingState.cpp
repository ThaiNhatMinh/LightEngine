#include "pch.h"
#include "OpenGLBlendingState.h"
#include "DataMap.h"
Light::render::OpenGLBlendingState::OpenGLBlendingState(BlendConfig config)
{
	

	this->Enable = config.Enable;
	this->sfactor = openGLfactor[config.sfactor];
	this->dfactor = openGLfactor[config.dfactor];
	this->func = openGLfunc[config.func];
}
