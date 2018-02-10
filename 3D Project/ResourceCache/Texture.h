#pragma once
#include "pch.h"
class Texture: public GLBO
{
public:
	virtual void Init();
	virtual void Shutdown();
	
	inline void Bind(GLuint tex_unit);;
	inline void UnBind();

private:
	string Name;
	GLuint iIndex;
	GLuint iWidth;
	GLuint iHeight;
	
};