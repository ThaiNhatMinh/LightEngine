#include "stdafx.h"
#include "OpenGLCompressTexture.h"
#include "DataMap.h"
using namespace Light;
render::OpenGLCompressTexture::OpenGLCompressTexture(UINT32 target, int level, int internalformat, int width, int height, int border, int imageSize, const void * data)
{
	W = width;
	H = height;
	glGenTextures(1, &m_iHandle);

	GLenum gltarget = openGLTexType[target];
	//target = GL_TEXTURE_2D;
	glBindTexture(gltarget, m_iHandle);


	glTexParameteri(gltarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(gltarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(gltarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(gltarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum internalFormat = openGLformat[internalformat];
	//internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;

	glCompressedTexImage2D(target, level, internalFormat, width, height, 0,imageSize , (GLvoid*)data);
	glGenerateMipmap(target);
	check_gl_error();


	W = width;
	H = height;
	target = gltarget;
}

render::OpenGLCompressTexture::~OpenGLCompressTexture()
{
	glDeleteTextures(1, &m_iHandle);
}
