#include "stdafx.h"
#include "OpenGLIndexBuffer.h"
#include "DataMap.h"
using namespace Light;
render::OpenGLIndexBuffer::OpenGLIndexBuffer(long long size, const void * pData)
{
	glGenBuffers(1, &m_iHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, pData, GL_STATIC_DRAW); // static for now, update late
}

render::OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &m_iHandle);
}

void Light::render::OpenGLIndexBuffer::SetData(long long size, const void * data, BufferUsage usage)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, openglBufferUsage[usage]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void * Light::render::OpenGLIndexBuffer::Map(BufferAccess access)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iHandle);
	return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, openglBufferAccess[access]);
}

bool Light::render::OpenGLIndexBuffer::UnMap()
{
	bool r = glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return r;
}