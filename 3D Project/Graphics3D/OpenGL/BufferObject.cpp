#include "pch.h"
#include "BufferObject.h"

BufferObject::BufferObject(GLenum target) :m_eTarget(target)
{
	glGenBuffers(1, &m_uiIndentity);
}

BufferObject::~BufferObject()
{
	glDeleteBuffers(1, &m_uiIndentity);
}

void BufferObject::Bind()
{
	glBindBuffer(m_eTarget, m_uiIndentity);
}

void BufferObject::UnBind()
{
	glBindBuffer(m_eTarget, m_uiIndentity);
}

void BufferObject::SetData(GLuint size, const GLvoid * data, GLenum type)
{
	glBufferData(m_eTarget, size, data, type);
}

void BufferObject::SetSubData(GLuint offset, GLuint size, const GLvoid * data)
{
	glBufferSubData(m_eTarget, offset, size, data);
}

BufferObject::BufferObject(BufferObject && other)
{
	m_eTarget = other.m_eTarget;
	m_uiIndentity = m_uiIndentity;
	other.m_uiIndentity = 0;
	printf("BufferObject\n");
}

BufferObject & BufferObject::operator=(BufferObject && other)
{
	m_eTarget = other.m_eTarget;
	m_uiIndentity = m_uiIndentity;
	other.m_uiIndentity = 0;

	return *this;
}
