#include "pch.h"
#include "OpenGLPixelShader.h"

using namespace Light;

render::OpenGLPixelShader::OpenGLPixelShader(const char * code)
{
	GLint check;
	char infoLog[512];

	m_iHandle = glCreateShader(GL_FRAGMENT_SHADER);
	const char* pvStr = code;
	glShaderSource(m_iHandle, 1, &pvStr, NULL);
	glCompileShader(m_iHandle);


	glGetShaderiv(m_iHandle, GL_COMPILE_STATUS, &check);
	if (check == GL_FALSE)
	{
		glGetShaderInfoLog(m_iHandle, 512, NULL, infoLog);
		E_ERROR("Pixel shader compile error: %s",infoLog);

	}
}

render::OpenGLPixelShader::~OpenGLPixelShader()
{
	glDeleteShader(m_iHandle);
}
