#include "stdafx.h"
#include "OpenGLFrameBuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLRenderBuffer.h"

static GLenum openGLAttachment[] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT , GL_DEPTH_STENCIL_ATTACHMENT };
static GLenum openglBuffer[] = { GL_FRONT_LEFT, GL_FRONT_RIGHT, GL_BACK_LEFT, GL_BACK_RIGHT, GL_FRONT, GL_BACK, GL_LEFT, GL_RIGHT };
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Light::render::OpenGLFrameBuffer::AttachRenderBuffer(Attachment attachment, RenderBuffer * pBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_iHandle);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, openGLAttachment[attachment], GL_RENDERBUFFER, static_cast<OpenGLRenderBuffer*>(pBuffer)->m_iHandle);
	this->CheckStatus();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::render::OpenGLFrameBuffer::DrawBuffer(Buffer buffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_iHandle);
	if (buffer == Buffer::BUFFER_MAX)
	{
		printf("%s:%d| Error unknow buffer", __FILE__, __LINE__);
	}
	else
	{
		glDrawBuffer(openglBuffer[buffer]);
	}
	this->CheckStatus();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::render::OpenGLFrameBuffer::ReadBuffer(Buffer buffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_iHandle);
	if (buffer == Buffer::BUFFER_MAX)
	{
		printf("%s:%d| Error unknow buffer", __FILE__, __LINE__);
	}
	else
	{
		glReadBuffer(openglBuffer[buffer]);
	}
	this->CheckStatus();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
