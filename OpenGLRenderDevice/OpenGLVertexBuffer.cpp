#include "stdafx.h"
#include "OpenGLVertexBuffer.h"
#include "DataMap.h"
using namespace Light;

render::OpenGLVertexBuffer::OpenGLVertexBuffer(long long size, const void * data)
{
	glGenBuffers(1, &m_iHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_iHandle);					// GL_ARRAY_BUFFER for now, update late
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);	// static for now, update late
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

render::OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1,&m_iHandle);
}

void Light::render::OpenGLVertexBuffer::SetData(long long size, const void * data, BufferUsage usage)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_iHandle);
	glBufferData(GL_ARRAY_BUFFER, size, data, openglBufferUsage[usage]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void * Light::render::OpenGLVertexBuffer::Map(BufferAccess access)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_iHandle);
	return glMapBuffer(GL_ARRAY_BUFFER, openglBufferAccess[access]);
}

bool Light::render::OpenGLVertexBuffer::UnMap()
{
	bool r =  glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return r;
}
