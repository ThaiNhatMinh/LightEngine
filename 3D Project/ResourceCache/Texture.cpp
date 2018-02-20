#include "pch.h"

Texture::Texture(const string & name, GLuint w, GLuint h):Name(name),iWidth(w),iHeight(h)
{

}

Texture::Texture(GLuint id, const string & name, GLuint w, GLuint h) : iIndex(id),Name(name), iWidth(w), iHeight(h)
{
}

void Texture::Init()
{
	glGenTextures(1, &iIndex);
	glBindTexture(GL_TEXTURE_2D, iIndex);
}

void Texture::Shutdown()
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

const string & Texture::GetName()
{
	return Name;
}
