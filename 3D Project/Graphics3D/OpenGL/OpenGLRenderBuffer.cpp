#include "pch.h"
#include "OpenGLRenderBuffer.h"

Light::render::OpenGLRenderBuffer::OpenGLRenderBuffer(int w, int h, InternalFormat format)
{
	static GLenum openGLformat[] = { GL_R8,
								GL_R8UI,
								GL_R8I,
								GL_R16UI,
								GL_R16I,
								GL_R32UI,
								GL_R32I,
								GL_RG8,
								GL_RG8UI,
								GL_RG8I,
								GL_RG16UI,
								GL_RG16I,
								GL_RG32UI,
								GL_RG32I,
								GL_RGB8,
								GL_RGB565,
								GL_RGBA8,
								GL_SRGB8_ALPHA8,
								GL_RGB5_A1,
								GL_RGBA4,
								GL_RGB10_A2,
								GL_RGBA8UI,
								GL_RGBA8I,
								GL_RGB10_A2UI,
								GL_RGBA16UI,
								GL_RGBA16I,
								GL_RGBA32I,
								GL_RGBA32UI,
								GL_DEPTH_COMPONENT16,
								GL_DEPTH_COMPONENT24,
								GL_DEPTH_COMPONENT32F,
								GL_DEPTH24_STENCIL8,
								GL_DEPTH32F_STENCIL8,
								GL_STENCIL_INDEX8 };


	glGenRenderbuffers(1, &m_iHandle);
	glBindRenderbuffer(GL_RENDERBUFFER, m_iHandle);
	glRenderbufferStorage(GL_RENDERBUFFER, openGLformat[format], w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Light::render::OpenGLRenderBuffer::~OpenGLRenderBuffer()
{
	glDeleteRenderbuffers(1, &m_iHandle);
}
