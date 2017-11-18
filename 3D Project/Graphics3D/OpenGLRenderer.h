#pragma once
#include "RenderAPI.h"



class OpenGLRenderer : public RenderAPICore
{
	friend class Scene;
private:
	const char*		RendererConfig = "Renderer.xml";
	string			m_RenderName;
	string			m_WindowName;
	vec4			m_ClearColor;
	vec4			m_Viewport;
	bool			m_HasInit;
	Windows			*m_pWindows;
	unsigned int	m_iClearFlag;
	GLenum			m_DrawMode;
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	virtual bool Init();
	virtual bool ShutDown();

	virtual bool HasInit();
	virtual void ClearBuffer();
	virtual void ClearColor();
	virtual void Clear();

	virtual void Draw(GLint first, GLsizei count, GLsizei primcount = 0);
	virtual void DrawElement(GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount = 0);

	virtual void SetClearFlag(unsigned int flag);
	virtual void SetTexture(Texture* p, GLuint tex_unit = 0);
	virtual void SetViewport(int x, int y, int weight, int height);
	virtual void SetViewport(vec4 v);
	virtual void SetDrawMode(GLenum mode);
	virtual void SetVertexArrayBuffer(GLuint vao);
	virtual vec4 GetViewport();

	virtual void SwapBuffer();

	Windows* GetWindow() {
		return m_pWindows;
	};

private:
	

	bool ReadConfig(string path);
};