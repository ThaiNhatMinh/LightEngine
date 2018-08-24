#include "pch.h"
#include "OpenGLVertexBuffer.h"

using namespace Light;

render::OpenGLVertexBuffer::OpenGLVertexBuffer(long long size, const void * data)
{
	glGenBuffers(1, &m_iHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_iHandle);					// GL_ARRAY_BUFFER for now, update late
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);	// static for now, update late
}

render::OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1,&m_iHandle);
}
