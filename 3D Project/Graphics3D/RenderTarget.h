#pragma once
#include "OpenGLRenderer.h"


class RenderTarget
{
public:
	RenderTarget(GLuint W,GLuint H);
	~RenderTarget();

	void Bind();
	void UnBind();
	
	void Render();
private:
	FrameBuffer m_framebuffer;
	RenderBuffer m_renderbuffer;
	VertexArray VAO;
	BufferObject VBO;
	Texture *m_RenderTexture;
	unsigned int rbo;
	
};