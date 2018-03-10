#pragma once
#include <gl\glew.h>


class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind();
	void UnBind();

	void SetAttibutePointer(GLuint loc, GLuint num, GLenum type, GLuint stride, GLuint offset);
	
private:
	VertexArray(const VertexArray& other) = delete;
	VertexArray(VertexArray&& other) = delete;
	VertexArray& operator=(const VertexArray& other) = delete;
	VertexArray& operator=(VertexArray&& other) = delete;

private:
	
	GLuint m_uiIndentity;
};