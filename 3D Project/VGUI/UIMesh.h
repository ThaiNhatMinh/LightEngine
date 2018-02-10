#pragma once


class UIMesh: public GLBO
{
public:
	virtual void Init();
	virtual void Shutdown();

	GLuint VAO;
	GLuint VBO;
};