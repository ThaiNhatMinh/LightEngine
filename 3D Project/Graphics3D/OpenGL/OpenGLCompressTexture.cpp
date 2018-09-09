#include "pch.h"
#include "OpenGLCompressTexture.h"
#include "DataMap.h"
using namespace Light;
render::OpenGLCompressTexture::OpenGLCompressTexture(const TextureCreateInfo & info):Texture(info)
{
	glGenTextures(1, &m_iHandle);

	GLenum target = openGLTexType[info.eTarget];
	//target = GL_TEXTURE_2D;
	glBindTexture(target, m_iHandle);


	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum internalFormat = openGLformat[info.iInternalFormat];
	//internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;

	glCompressedTexImage2D(target, 0, internalFormat, info.uiWidth, info.uiHeight, 0, info.iLevel, (GLvoid*)info.pData);
	glGenerateMipmap(target);
	check_gl_error();
}

render::OpenGLCompressTexture::~OpenGLCompressTexture()
{
	glDeleteTextures(1, &m_iHandle);
}
