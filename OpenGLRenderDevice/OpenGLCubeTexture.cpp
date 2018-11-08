#include "stdafx.h"
#include "OpenGLCubeTexture.h"
#include "DataMap.h"

Light::render::OpenGLCubeTexture::OpenGLCubeTexture(UINT32 target, int level, int internalformat, int width, int height, int border, int format, int type, const void * data)
{
	glGenTextures(1, &m_iHandle);
	GLenum gltarget = openGLTexType[target];
	glBindTexture(gltarget, m_iHandle);
	// format cube map texture
	glTexParameteri(gltarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(gltarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(gltarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(gltarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(gltarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	unsigned char** pData = (unsigned char**)data;
	GLenum glinternalFormat = openGLformat[internalformat];
	GLenum glformat = openGLformat[format];
	GLenum gltype = openGLNumber[type];

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, glinternalFormat, width, height, border, glformat, gltype, pData[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, level, glinternalFormat, width, height, border, glformat, gltype, pData[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, level, glinternalFormat, width, height, border, glformat, gltype, pData[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, level, glinternalFormat, width, height, border, glformat, gltype, pData[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, level, glinternalFormat, width, height, border, glformat, gltype, pData[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, level, glinternalFormat, width, height, border, glformat, gltype, pData[5]);

	W = width;
	H = height;
	target = gltarget;


}


Light::render::OpenGLCubeTexture::~OpenGLCubeTexture()
{
	glDeleteTextures(1, &m_iHandle);
}
