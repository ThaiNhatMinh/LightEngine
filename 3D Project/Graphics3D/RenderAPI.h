#pragma once

#include "pch.h"


class RenderAPICore
{
public:
	RenderAPICore() {};
	virtual ~RenderAPICore() {};

	virtual bool HasInit() = 0;
	virtual void ClearBuffer() = 0;
	virtual void ClearColor() = 0;
	virtual void Clear() = 0;

	virtual void Draw(GLint first, GLsizei count, GLsizei primcount = 0) = 0;
	virtual void DrawElement(GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount = 0) = 0;

	virtual void SetClearFlag(unsigned int flag) = 0;
	virtual void SetViewport(int x, int y, int weight, int height) = 0;
	virtual void SetViewport(vec4 v) = 0;
	virtual void SetDrawMode(GLenum mode) = 0;
	virtual void SetVertexArrayBuffer(GLuint vao) = 0;
	virtual vec4 GetViewport() = 0;

	virtual void SwapBuffer() = 0;
};
