#include "pch.h"
#include "OpenGLCubeTexture.h"
#include "DataMap.h"

Light::render::OpenGLCubeTexture::OpenGLCubeTexture(const TextureCreateInfo& info):Texture(info)
{
	glGenTextures(1, &m_iHandle);

	glBindTexture(openGLTexType[info.eTarget], m_iHandle);
	// format cube map texture
	glTexParameteri(openGLTexType[info.eTarget], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(openGLTexType[info.eTarget], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(openGLTexType[info.eTarget], GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(openGLTexType[info.eTarget], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(openGLTexType[info.eTarget], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	unsigned char** pData = (unsigned char**)info.pData;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, info.iLevel, openGLformat[info.iInternalFormat], info.uiWidth, info.uiHeight, 0, openGLformat[info.eFormat], openGLNumber[info.eType], pData[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, info.iLevel, openGLformat[info.iInternalFormat], info.uiWidth, info.uiHeight, 0, openGLformat[info.eFormat], openGLNumber[info.eType], pData[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, info.iLevel, openGLformat[info.iInternalFormat], info.uiWidth, info.uiHeight, 0, openGLformat[info.eFormat], openGLNumber[info.eType], pData[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, info.iLevel, openGLformat[info.iInternalFormat], info.uiWidth, info.uiHeight, 0, openGLformat[info.eFormat], openGLNumber[info.eType], pData[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, info.iLevel, openGLformat[info.iInternalFormat], info.uiWidth, info.uiHeight, 0, openGLformat[info.eFormat], openGLNumber[info.eType], pData[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, info.iLevel, openGLformat[info.iInternalFormat], info.uiWidth, info.uiHeight, 0, openGLformat[info.eFormat], openGLNumber[info.eType], pData[5]);

}


Light::render::OpenGLCubeTexture::~OpenGLCubeTexture()
{
	glDeleteTextures(1, &m_iHandle);
}
