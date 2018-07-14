#include "pch.h"
#include "OpenGLCompressTexture.h"
using namespace Light;
render::OpenGLCompressTexture::OpenGLCompressTexture(const TextureCreateInfo & info):Texture(info)
{
	glGenTextures(1, &m_iHandle);

	glBindTexture(GL_TEXTURE_2D, m_iHandle);
	/*m_TexInfo.uiWidth = info.uiWidth;
	m_TexInfo.uiHeight = info.uiHeight;
	m_TexInfo.iInternalFormat = info.iInternalFormat;*/


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glCompressedTexImage2D(GL_TEXTURE_2D, 0, info.iInternalFormat, info.uiWidth, info.uiHeight, 0, info.iLevel, (GLvoid*)info.pData);
	glGenerateMipmap(GL_TEXTURE_2D);
}

render::OpenGLCompressTexture::~OpenGLCompressTexture()
{
	glDeleteTextures(1, &m_iHandle);
}
