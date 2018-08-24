
#include <pch.h>

#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(GLuint Width, GLuint Height)
{
	glGenRenderbuffers(1, &m_uiIndentity);
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
	//UnBind();
}

RenderBuffer::~RenderBuffer()
{
	glDeleteRenderbuffers(1, &m_uiIndentity);
	//cout << "RenderBuffer::~RenderBuffer()\n";
}

void RenderBuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_uiIndentity);
}

void RenderBuffer::UnBind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

GLuint RenderBuffer::GetIndentity()
{
	return m_uiIndentity;
}
