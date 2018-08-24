#include "pch.h"
#include "OpenGLIndexBuffer.h"
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
