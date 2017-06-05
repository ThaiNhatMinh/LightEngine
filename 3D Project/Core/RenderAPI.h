#pragma once

#include "..\pch.h"


class RenderAPICore : public Singleton<RenderAPICore>
{
public:
	RenderAPICore() {};
	~RenderAPICore() {};
	virtual void  onStartUp() = 0;
	virtual void  onShutDown() = 0;


	virtual void SetViewport(int x, int y, int weight, int height) = 0;
	virtual void ClearBuffer() = 0;
	virtual void SetTexture(Texture* p) = 0;
	virtual void DrawMesh(Mesh* p) = 0;
	virtual void Draw(GLint first, GLsizei count, GLsizei primcount=0) = 0;
	virtual void DrawElement(GLsizei count,	GLenum type,const GLvoid * indices, GLsizei primcount=0) = 0;
private:
	string	m_RenderName;
	vec3	m_ClearColor;
};