#include "pch.h"
#include "OpenGLPipelineParam.h"
using namespace Light;
render::OpenGLPipelineParam::OpenGLPipelineParam(GLuint location, const char * name):m_iLocation(location),m_Name(name)
{

}

void render::OpenGLPipelineParam::SetAsInt(int value)
{
	glUniform1i(m_iLocation, value);
}

void render::OpenGLPipelineParam::SetAsFloat(float value)
{
	glUniform1f(m_iLocation, value);
}

void render::OpenGLPipelineParam::SetAsMat4(float * value)
{
	glUniformMatrix4fv(m_iLocation, 1, GL_FALSE, value);
}

void render::OpenGLPipelineParam::SetAsVec3(float * value)
{
	glUniform3f(m_iLocation, value[0],value[1],value[2]);
}

void render::OpenGLPipelineParam::SetAsVec2(float * value)
{
	glUniform2f(m_iLocation, value[0], value[1]);
}
