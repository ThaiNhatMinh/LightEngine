#pragma once
#include "pch.h"
class Texture: public GLBO
{
public:
	Texture(const string& name, GLuint w, GLuint h);
	Texture(GLuint id,const string& name, GLuint w, GLuint h);
	~Texture() = default;
	virtual void Init();
	virtual void Shutdown();
	
	void Bind(GLuint tex_unit = 0);
	void UnBind();

	GLuint GetWidth();
	GLuint GetHeight();

	const string& GetName();
private:
	string Name;
	GLuint iIndex;
	GLuint iWidth;
	GLuint iHeight;
	
};