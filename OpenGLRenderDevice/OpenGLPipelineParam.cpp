#include "stdafx.h"
#include "OpenGLPipelineParam.h"
using namespace Light;
render::OpenGLPipelineParam::OpenGLPipelineParam(GLuint location, const char * name):m_iLocation(location),m_Name(name)
{

}

void render::OpenGLPipelineParam::SetAsInt(const int value)
{
	glUniform1i(m_iLocation, value);
}

void render::OpenGLPipelineParam::SetAsFloat(const float value)
{
	glUniform1f(m_iLocation, value);
}

void render::OpenGLPipelineParam::SetAsMat4(const float * value)
{
	glUniformMatrix4fv(m_iLocation, 1, GL_FALSE, value);
}

void render::OpenGLPipelineParam::SetAsVec3(const float * value)
{
	glUniform3f(m_iLocation, value[0],value[1],value[2]);
}

void render::OpenGLPipelineParam::SetAsVec2(const float * value)
{
	glUniform2f(m_iLocation, value[0], value[1]);
}

void Light::render::OpenGLPipelineParam::SetAsVec4(const float * value)
{
	glUniform4f(m_iLocation, value[0], value[1], value[2], value[3]);
}

void Light::render::OpenGLPipelineParam::SetAsFloatV(const float* value, int num)
{
	glUniform1fv(m_iLocation, num, value);
}

void Light::render::OpenGLPipelineParam::SetAsMat4V(const float * value, int num)
{
	glUniformMatrix4fv(m_iLocation, num, GL_FALSE, value);
}

void Light::render::OpenGLPipelineParam::SetAsVec3V(const float * value, int num)
{
	glUniform3fv(m_iLocation, num, value);
}

void Light::render::OpenGLPipelineParam::SetAsVec2V(const float * value, int num)
{
	glUniform2fv(m_iLocation, num, value);
}
