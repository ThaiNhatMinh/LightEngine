#include "pch.h"

CubeTexture::CubeTexture(const TextureCreateInfo & TexCreateInfo)
{
	m_TexInfo = TexCreateInfo;
	glGenTextures(1, &iIndex);
	check_gl_error();
	Bind();
	check_gl_error();
	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	check_gl_error();
	unsigned char** pData = (unsigned char**)m_TexInfo.pData;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_TexInfo.iLevel, m_TexInfo.iInternalFormat, m_TexInfo.uiWidth, m_TexInfo.uiHeight, 0, m_TexInfo.eFormat, m_TexInfo.eType, pData[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,m_TexInfo.iLevel, m_TexInfo.iInternalFormat, m_TexInfo.uiWidth, m_TexInfo.uiHeight, 0, m_TexInfo.eFormat, m_TexInfo.eType,  pData[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,m_TexInfo.iLevel, m_TexInfo.iInternalFormat, m_TexInfo.uiWidth, m_TexInfo.uiHeight, 0, m_TexInfo.eFormat, m_TexInfo.eType,  pData[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,m_TexInfo.iLevel, m_TexInfo.iInternalFormat, m_TexInfo.uiWidth, m_TexInfo.uiHeight, 0, m_TexInfo.eFormat, m_TexInfo.eType,  pData[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,m_TexInfo.iLevel, m_TexInfo.iInternalFormat, m_TexInfo.uiWidth, m_TexInfo.uiHeight, 0, m_TexInfo.eFormat, m_TexInfo.eType,  pData[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,m_TexInfo.iLevel, m_TexInfo.iInternalFormat, m_TexInfo.uiWidth, m_TexInfo.uiHeight, 0, m_TexInfo.eFormat, m_TexInfo.eType,  pData[5]);

	
	UnBind();

}
