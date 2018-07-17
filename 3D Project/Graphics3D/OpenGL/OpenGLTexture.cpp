#include "pch.h"
#include "OpenGLTexture.h"
using namespace Light;
render::OpenGLTexture::OpenGLTexture(const TextureCreateInfo & info) :Texture(info)
{
	glGenTextures(1, &m_iHandle);
	glBindTexture(info.eTarget, m_iHandle);

	glTexParameteri(info.eTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(info.eTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(info.eTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(info.eTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(info.eTarget, info.iLevel, info.iInternalFormat, info.uiWidth, info.uiHeight, 0, info.eFormat, info.eType, info.pData);
	glGenerateMipmap(info.eTarget);
}

render::OpenGLTexture::~OpenGLTexture()
{
	E_DEBUG("OpenGLTexture::~OpenGLTexture()");
	glDeleteTextures(1, &m_iHandle);
}
