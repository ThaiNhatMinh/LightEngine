#include <pch.h>
#include "FrameBuffer.h"
#include "RenderBuffer.h"


FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1,&m_uiIndentity);
	
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1,&m_uiIndentity);
	
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiIndentity);
}

void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::AttachTexture(GLenum attachment, Texture *pTexrure)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, pTexrure->GetIndentity(), 0);
}

void FrameBuffer::AttachDepthStencil(GLenum attachment, RenderBuffer & rb)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rb.GetIndentity());
}
