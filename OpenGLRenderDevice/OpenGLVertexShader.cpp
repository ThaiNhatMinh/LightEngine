#include "stdafx.h"
#include "OpenGLVertexShader.h"
using namespace Light;
render::OpenGLVertexShader::OpenGLVertexShader(const char * code)
{
	GLint check;
	char infoLog[512];

	m_iHandle = glCreateShader(GL_VERTEX_SHADER);
	const char* pvStr = code;
	glShaderSource(m_iHandle, 1, &pvStr, NULL);
	glCompileShader(m_iHandle);


	glGetShaderiv(m_iHandle, GL_COMPILE_STATUS, &check);
	if (check == GL_FALSE)
	{
		glGetShaderInfoLog(m_iHandle, 512, NULL, infoLog);
		printf("Vertex shader compile error: %s",infoLog);

	}
}

render::OpenGLVertexShader::~OpenGLVertexShader()
{
	glDeleteShader(m_iHandle);
}
