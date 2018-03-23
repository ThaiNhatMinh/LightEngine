#pragma once
#include <pch.h>

class Texture :public IResource
{
public:
	struct TextureCreateInfo
	{
		GLenum eTarget;				//  GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_1D_ARRAY, GL_PROXY_TEXTURE_1D_ARRAY, GL_TEXTURE_RECTANGLE, GL_PROXY_TEXTURE_RECTANGLE
		GLint iLevel;				// Specifies the level-of-detail number.
		GLint iInternalFormat;		// Specifies the number of color components in the texture.
		GLuint uiWidth, uiHeight;	// Specifies the width/height of the texture image
		GLenum eFormat;				// Specifies the format of the pixel data.
		GLenum eType;				// Specifies the data type of the pixel data
		void* pData;				// Specifies a pointer to the image data in memory. 
		TextureCreateInfo();
	};
public:
	Texture(const TextureCreateInfo& TexCreateInfo);
	~Texture();

	void Bind(GLuint tex_unit = 0);
	void UnBind();

	GLuint GetWidth();
	GLuint GetHeight();

	GLuint GetIndentity();
protected:
	Texture() = default;
private:
	Texture(const Texture& other) = delete;
	Texture(Texture&& other) = delete;
	Texture& operator=(const Texture& other) = delete;
	Texture& operator=(Texture&& other) = delete;
protected:
	TextureCreateInfo m_TexInfo;
	GLuint iIndex;
};