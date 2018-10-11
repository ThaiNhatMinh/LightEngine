#include "stdafx.h"
#include "OpenGLTexture.h"
#include "DataMap.h"
using namespace Light;
render::OpenGLTexture::OpenGLTexture(const TextureCreateInfo & info) :Texture(info)
{
	glGenTextures(1, &m_iHandle);

	GLenum target = openGLTexType[info.eTarget];
	glBindTexture(target, m_iHandle);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum internalFormat = openGLformat[info.iInternalFormat];
	GLenum format = openGLformat[info.eFormat];
	GLenum type = openGLNumber[info.eType];

	glTexImage2D(target, info.iLevel, internalFormat, info.uiWidth, info.uiHeight, 0,format, type, info.pData);
	glGenerateMipmap(target);
	glBindTexture(target, 0);
	check_gl_error();
}

render::OpenGLTexture::~OpenGLTexture()
{
	
	glDeleteTextures(1, &m_iHandle);
}
