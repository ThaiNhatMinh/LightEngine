#pragma once
#include <gl\glew.h>

class BufferObject
{
public:
	BufferObject(GLenum target);
	~BufferObject();

	void Bind();
	void UnBind();

	void SetData(GLuint size, const GLvoid* data,GLenum usage);
	void SetSubData(GLuint offset, GLuint size, const GLvoid* data);
private:
	BufferObject(const BufferObject& other) = delete;
	BufferObject(BufferObject&& other) = delete;
	BufferObject& operator=(const BufferObject& other) = delete;
	BufferObject& operator=(BufferObject&& other) = delete;

private:
	GLenum m_eTarget;
	GLuint m_uiIndentity;
};