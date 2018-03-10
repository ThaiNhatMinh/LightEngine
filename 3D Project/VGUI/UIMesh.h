#pragma once


class UIMesh
{
public:
	virtual void Init();
	virtual void Shutdown();

	GLuint VAO;
	GLuint VBO;
};