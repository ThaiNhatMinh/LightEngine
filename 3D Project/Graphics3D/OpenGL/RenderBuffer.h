#pragma once
#include "FrameBuffer.h"
class RenderBuffer
{
public:
	RenderBuffer(GLuint Width, GLuint Height);
	~RenderBuffer();


	void Bind();
	void UnBind();

	GLuint GetIndentity();
private:
	
	GLuint m_uiIndentity;
};