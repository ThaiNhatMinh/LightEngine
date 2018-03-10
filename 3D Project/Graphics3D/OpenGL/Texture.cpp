#include "pch.h"

Texture::Texture(GLuint w, GLuint h, GLuint bpp, unsigned char* data):iWidth(w),iHeight(h), iBpp(bpp)
{
	glGenTextures(1, &iIndex);
	
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLenum type = GL_RGBA;
	if (bpp == 24) type = GL_RGB;
	else if (bpp == 3) type = GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, type, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture()
{
	glDeleteTextures(1, &iIndex);
}


void Texture::Bind(GLuint tex_unit) {
	glActiveTexture(GL_TEXTURE0 + tex_unit);
	glBindTexture(GL_TEXTURE_2D, iIndex);
}

void Texture::UnBind() { glBindTexture(GL_TEXTURE_2D, 0); }

GLuint Texture::GetWidth()
{
	return iWidth;
}

GLuint Texture::GetHeight()
{
	return iHeight;
}
