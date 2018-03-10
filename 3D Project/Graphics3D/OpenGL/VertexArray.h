#pragma once
#include <gl\glew.h>


class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind();
	void UnBind();

	void SetAttibutePointer(GLuint loc, GLuint num, GLenum type, GLuint stride, GLuint offset,bool normalize = false);
	VertexArray(VertexArray&& other);
	VertexArray& operator=(VertexArray&& other);
private:
	VertexArray(const VertexArray& other) = delete;
	
	VertexArray& operator=(const VertexArray& other) = delete;
	

private:
	
	GLuint m_uiIndentity;
};