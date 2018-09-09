#include "pch.h"
#include "OpenGLRasterState.h"
#include "DataMap.h"

Light::render::OpenGLRasterState::OpenGLRasterState(CullFaceConfig config)
{
	

	this->CullEnable = config.CullEnable;
	this->CullFace = openGLFace[config.CullFace];
	this->FillMode = openGLFill[config.FillMode];
	this->FrontFaceWinding = openGLWiding[config.FrontFaceWinding];

}
