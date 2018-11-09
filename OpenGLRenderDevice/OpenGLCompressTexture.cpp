#include "stdafx.h"
#include "OpenGLCompressTexture.h"
#include "DataMap.h"
using namespace Light;
render::OpenGLCompressTexture::OpenGLCompressTexture(UINT32 target, int level, int internalformat, int width, int height, int border, int imageSize, const void * data)
{
	glGenTextures(1, &m_iHandle);

	GLenum gltarget = openGLTexType[target];
	glBindTexture(gltarget, m_iHandle);


	glTexParameteri(gltarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(gltarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(gltarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(gltarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum glinternalFormat = openGLformat[internalformat];

	glCompressedTexImage2D(gltarget, 0, glinternalFormat, width, height, border,imageSize , (GLvoid*)data);
	glGenerateMipmap(gltarget);
	check_gl_error();


	W = width;
	H = height;
	this->target = gltarget;
}

render::OpenGLCompressTexture::~OpenGLCompressTexture()
{
	glDeleteTextures(1, &m_iHandle);
}
