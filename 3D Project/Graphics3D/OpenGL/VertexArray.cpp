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

VertexArray::VertexArray(VertexArray && other)
{
	m_uiIndentity = other.m_uiIndentity;
	other.m_uiIndentity = 0;
	printf("VertexArray\n");
}

VertexArray & VertexArray::operator=(VertexArray && other)
{
	m_uiIndentity = other.m_uiIndentity;
	other.m_uiIndentity = 0;
	return *this;
}
