#pragma once
#include "RenderAPI.h"
#include <GLFW\glfw3.h>




class OpenGLRenderer : public RenderAPICore,public ISubSystem
{
	friend class Scene;
private:
	string			m_RenderName;
	vec4			m_ClearColor;
	vec4			m_Viewport;
	bool			m_HasInit;
	unsigned int	m_iClearFlag;
	GLenum			m_DrawMode;
	GLFWwindow*		m_glfwWindow;
public:
	OpenGLRenderer(Context* c);
	~OpenGLRenderer();

	//virtual void Init(Context* c);
	//virtual void ShutDown();

	virtual bool HasInit();
	virtual void ClearBuffer();
	virtual void ClearColor();
	virtual void Clear();

	virtual void Draw(GLint first, GLsizei count, GLsizei primcount = 0);
	virtual void DrawElement(GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount = 0);
	
	virtual void SetClearFlag(unsigned int flag);
	virtual void SetViewport(int x, int y, int weight, int height);
	virtual void SetViewport(vec4 v);
	virtual void SetDrawMode(GLenum mode);
	virtual void SetVertexArrayBuffer(GLuint vao);
	virtual vec4 GetViewport();

	virtual void SwapBuffer();

	virtual char* GetName()override;
private:
	

	bool ReadConfig(tinyxml2::XMLElement *pData);
};