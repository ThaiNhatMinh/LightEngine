#include "pch.h"
#include "OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer():m_RenderName("OpenGL"),m_ClearColor(1.0f,1.0f,1.0f,1.0f),m_HasInit(0),m_iClearFlag(0),m_DrawMode(GL_TRIANGLES)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
	ShutDown();
}

bool OpenGLRenderer::ReadConfig(tinyxml2::XMLElement *pRenderer)
{
	/*
	
	*/

	tinyxml2::XMLElement* pClearColor = pRenderer->FirstChildElement("ClearColor");

	m_ClearColor.x = pClearColor->DoubleAttribute("r", 0.3);
	m_ClearColor.y = pClearColor->DoubleAttribute("g", 0.3);
	m_ClearColor.z = pClearColor->DoubleAttribute("b", 0.3);
	m_ClearColor.w = pClearColor->DoubleAttribute("a", 1.0);

	

	return true;
}

void OpenGLRenderer::Init(Context* c)
{
	if (!ReadConfig(c->GetElement("Graphic")))
	{
		E_ERROR("Failed to init OpenGL Renderer");
	}

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) 
	{
		string Error = string("glewInit failed! ");
		E_ERROR(Error);
		return;
	}

	vec2 size = c->m_pWindows->GetWindowSize();
	m_glfwWindow = c->m_pWindows->Window();


	m_Viewport = vec4(0, 0, size.x, size.y);
	// Define the viewport dimensions
	glViewport(m_Viewport.x, m_Viewport.y, m_Viewport.z, m_Viewport.w);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	//glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	SetClearFlag(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glfwSwapInterval(0);

	c->m_pRenderer = std::unique_ptr<OpenGLRenderer>(this);
	

}

void OpenGLRenderer::ShutDown()
{

}

bool OpenGLRenderer::HasInit()
{
	return m_HasInit;
}

void OpenGLRenderer::ClearBuffer()
{
	glClear(m_iClearFlag);
}

void OpenGLRenderer::ClearColor()
{
	glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
}

void OpenGLRenderer::Clear()
{
	ClearBuffer();
	ClearColor();
}

void OpenGLRenderer::Draw(GLint first, GLsizei count, GLsizei primcount)
{
	if (primcount) glDrawArraysInstanced(m_DrawMode, first, count, primcount);
	else glDrawArrays(m_DrawMode, first, count);
}

void OpenGLRenderer::DrawElement(GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount)
{
	if (primcount) glDrawElementsInstanced(m_DrawMode, count, type, indices, primcount);
	else glDrawElements(m_DrawMode, count, type, indices);
}



void OpenGLRenderer::SetClearFlag(unsigned int flag)
{
	m_iClearFlag = flag;
}

void OpenGLRenderer::SetViewport(int x, int y, int weight, int height)
{
	m_Viewport = vec4(x, y, weight, height);
	glViewport(x, y, weight, height);
}

void OpenGLRenderer::SetViewport(vec4 v)
{
	m_Viewport = v;
	glViewport(v.x, v.y, v.z, v.w);
}

void OpenGLRenderer::SetDrawMode(GLenum mode)
{
	m_DrawMode = mode;
}

void OpenGLRenderer::SetVertexArrayBuffer(GLuint vao)
{
	glBindVertexArray(vao);
}

vec4 OpenGLRenderer::GetViewport()
{
	return m_Viewport;
}

void OpenGLRenderer::SwapBuffer()
{
	glfwSwapBuffers(m_glfwWindow);
	
}
