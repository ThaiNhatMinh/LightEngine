#include <pch.h>

Texture::Texture(const std::string& path, const TextureCreateInfo& TexCreateInfo):IResource(path),m_TexInfo(TexCreateInfo)
{
	glGenTextures(1, &iIndex);

	Bind();

	glTexParameteri(m_TexInfo.eTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_TexInfo.eTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(m_TexInfo.eTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_TexInfo.eTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//GLenum type = GL_RGBA;
	//if (m_TexInfo.uiBpp == 24) type = GL_RGB;
	//else if (m_TexInfo.uiBpp == 3) type = GL_RGB;
	glTexImage2D(m_TexInfo.eTarget, m_TexInfo.iLevel, m_TexInfo.iInternalFormat, m_TexInfo.uiWidth, m_TexInfo.uiHeight, 0, m_TexInfo.eFormat, m_TexInfo.eType, m_TexInfo.pData);
	glGenerateMipmap(m_TexInfo.eTarget);
}

Texture::~Texture()
{
	glDeleteTextures(1, &iIndex);
}


void Texture::Bind(GLuint tex_unit) {
	glActiveTexture(GL_TEXTURE0 + tex_unit);
	glBindTexture(m_TexInfo.eTarget, iIndex);
}

void Texture::UnBind() { glBindTexture(m_TexInfo.eTarget, 0); }


GLuint Texture::GetWidth()
{
	return m_TexInfo.uiWidth;
}

GLuint Texture::GetHeight()
{
	return m_TexInfo.uiHeight;
}

GLuint Texture::GetIndentity()
{
	return iIndex;
}

Texture::TextureCreateInfo::TextureCreateInfo() :uiWidth(0), uiHeight(0), iInternalFormat(GL_RGB), eFormat(GL_RGB), pData(nullptr), eTarget(GL_TEXTURE_2D), iLevel(0), eType(GL_UNSIGNED_BYTE)
{
}