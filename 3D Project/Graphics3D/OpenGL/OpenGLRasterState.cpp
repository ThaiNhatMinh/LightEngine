#include "pch.h"
#include "OpenGLRasterState.h"

Light::render::OpenGLRasterState::OpenGLRasterState(CullFaceConfig config)
{
	static GLenum openGLWiding[] = { GL_CW, GL_CCW };
	static GLenum openGLFace[] = { GL_FRONT, GL_BACK, GL_FRONT_AND_BACK };
	static GLenum openGLFill[] = { GL_POINT,GL_LINE, GL_FILL };

	this->CullEnable = config.CullEnable;
	this->CullFace = openGLFace[config.CullFace];
	this->FillMode = openGLFill[config.FillMode];
	this->FrontFaceWinding = openGLWiding[config.FrontFaceWinding];

}
