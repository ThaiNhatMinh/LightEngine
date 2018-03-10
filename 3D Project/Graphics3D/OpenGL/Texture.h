#pragma once
#include "pch.h"

class Texture :public IResource
{
public:
	Texture(GLuint w, GLuint h, GLuint bpp, unsigned char* data);
	~Texture();
	
	void Bind(GLuint tex_unit = 0);
	void UnBind();

	GLuint GetWidth();
	GLuint GetHeight();

protected:
	Texture()=default;
private:
	Texture(const Texture& other) = delete;
	Texture(Texture&& other) = delete;
	Texture& operator=(const Texture& other) = delete;
	Texture& operator=(Texture&& other) = delete;
protected:
	GLuint iIndex;
	GLuint iWidth;
	GLuint iHeight;
	GLuint iBpp;
};