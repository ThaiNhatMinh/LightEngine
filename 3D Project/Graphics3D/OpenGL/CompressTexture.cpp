#include <pch.h>

CompressTexture::CompressTexture(GLint internalformat, GLint w, GLint h, GLint size, GLubyte* data)
{
	glGenTextures(1, &iIndex);

	Bind();
	iWidth = w;
	iHeight = h;
	iBpp = 0;


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, internalformat, w, h, 0, size, (GLvoid*)data);
	glGenerateMipmap(GL_TEXTURE_2D);
}
