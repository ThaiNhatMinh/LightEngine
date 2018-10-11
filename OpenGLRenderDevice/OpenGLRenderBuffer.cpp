#include "stdafx.h"
#include "OpenGLRenderBuffer.h"
#include "DataMap.h"
Light::render::OpenGLRenderBuffer::OpenGLRenderBuffer(int w, int h, ColorFormat format)
{
	glGenRenderbuffers(1, &m_iHandle);
	glBindRenderbuffer(GL_RENDERBUFFER, m_iHandle);
	glRenderbufferStorage(GL_RENDERBUFFER, openGLformat[format], w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Light::render::OpenGLRenderBuffer::~OpenGLRenderBuffer()
{
	glDeleteRenderbuffers(1, &m_iHandle);
}
