#include "stdafx.h"
#include "OpenGLTexture.h"
#include "DataMap.h"
using namespace Light;
render::OpenGLTexture::OpenGLTexture(UINT32 target, int level, int internalformat, int width, int height, int border, int format, int type, const void * data)
{
	glGenTextures(1, &m_iHandle);

	GLenum gltarget = openGLTexType[target];
	glBindTexture(gltarget, m_iHandle);

	glTexParameteri(gltarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(gltarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(gltarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(gltarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum glinternalFormat = openGLformat[internalformat];
	GLenum glformat = openGLformat[format];
	GLenum gltype = openGLNumber[type];

	glTexImage2D(gltarget, level, glinternalFormat, width, height, border, glformat, gltype, data);
	//glGenerateMipmap(gltarget);
	glBindTexture(gltarget, 0);
	check_gl_error();

	W = width;
	H = height;
	this->target = gltarget;

}

render::OpenGLTexture::~OpenGLTexture()
{
	
	glDeleteTextures(1, &m_iHandle);
}
