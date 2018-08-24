#pragma once
class RenderBuffer;
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind();
	void UnBind();
	void AttachTexture(GLenum attachment, Texture*);
	void AttachDepthStencil(GLenum attachment, RenderBuffer& rb);
private:
	GLuint m_uiIndentity;

};