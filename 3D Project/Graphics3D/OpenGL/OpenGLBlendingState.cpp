#include "pch.h"
#include "OpenGLBlendingState.h"

Light::render::OpenGLBlendingState::OpenGLBlendingState(BlendConfig config)
{
	static GLenum openGLfactor[] = { GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA,GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA };
	static GLenum openGLfunc[] = { GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX };

	this->Enable = config.Enable;
	this->sfactor = openGLfactor[config.sfactor];
	this->dfactor = openGLfactor[config.dfactor];
	this->func = openGLfunc[config.func];
}
