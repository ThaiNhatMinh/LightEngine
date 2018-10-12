#include "stdafx.h"
#include "OpenGLFrameBuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLRenderBuffer.h"

static GLenum openGLAttachment[] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT , GL_DEPTH_STENCIL_ATTACHMENT };

Light::render::OpenGLFrameBuffer::OpenGLFrameBuffer()
{
	glGenFramebuffers(1, &m_iHandle);
}

Light::render::OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
	glDeleteFramebuffers(1, &m_iHandle);
}

void Light::render::OpenGLFrameBuffer::AttachTexture(Attachment attachment, Texture * pTex, int level)
{
	glBindFramebuffer(GL_FRAMEBUFFER,m_iHandle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, openGLAttachment[attachment], GL_TEXTURE_2D, static_cast<OpenGLTexture*>(pTex)->m_iHandle, level);
	this->CheckStatus();

}

void Light::render::OpenGLFrameBuffer::AttachRenderBuffer(Attachment attachment, RenderBuffer * pBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_iHandle);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, openGLAttachment[attachment], GL_RENDERBUFFER, static_cast<OpenGLRenderBuffer*>(pBuffer)->m_iHandle);
	this->CheckStatus();
}

void Light::render::OpenGLFrameBuffer::Begin()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_iHandle);
}

void Light::render::OpenGLFrameBuffer::End()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Light::render::OpenGLFrameBuffer::CheckStatus()
{
	static const char* errorStr[] = { "GL_INVALID_ENUM is generated if target is not one of the accepted tokens.",
	"GL_INVALID_ENUM is generated if renderbuffertarget is not GL_RENDERBUFFER.", "GL_INVALID_OPERATION is generated if zero is bound to target" };

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		GLenum e = glGetError();
		if (e == GL_INVALID_ENUM) printf(errorStr[0]);
		else if (e == GL_INVALID_OPERATION) printf(errorStr[2]);
	}
}
