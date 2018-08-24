#include <pch.h>

CompressTexture::CompressTexture(const std::string& name,const TextureCreateInfo& info):Texture(name)
{
	glGenTextures(1, &iIndex);

	Bind();
	m_TexInfo.uiWidth = info.uiWidth;
	m_TexInfo.uiHeight = info.uiHeight;
	m_TexInfo.iInternalFormat = info.iInternalFormat;


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, info.iInternalFormat, info.uiWidth, info.uiHeight, 0, info.iLevel, (GLvoid*)info.pData);
	glGenerateMipmap(GL_TEXTURE_2D);
}
