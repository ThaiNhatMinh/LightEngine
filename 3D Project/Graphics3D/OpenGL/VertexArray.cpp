#include "pch.h"
#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_uiIndentity);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_uiIndentity);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_uiIndentity);
}

void VertexArray::UnBind()
{
	glBindVertexArray(0);
}

void VertexArray::SetAttibutePointer(GLuint loc, GLuint num, GLenum type, GLuint stride, GLuint offset,bool normalize)
{
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, num, type, normalize, stride, (GLvoid*)offset);
}
