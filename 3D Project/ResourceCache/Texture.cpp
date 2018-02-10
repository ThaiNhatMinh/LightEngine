#include "pch.h"

void Texture::Init()
{
	glGenTextures(1, &iIndex);
	glBindTexture(GL_TEXTURE_2D, iIndex);
}

void Texture::Shutdown()
{
	glDeleteTextures(1, &iIndex);
}

inline void Texture::Bind(GLuint tex_unit) {
	glActiveTexture(GL_TEXTURE0 + tex_unit);
	glBindTexture(GL_TEXTURE_2D, iIndex);
}

inline void Texture::UnBind() { glBindTexture(GL_TEXTURE_2D, 0); }
