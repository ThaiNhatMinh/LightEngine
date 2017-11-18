#pragma once
#pragma once

#include "pch.h"

// Shader with texture and lighting
class SkeShader : public Shader
{
private:
	GLuint m_BaseTransformLocation;
public:
	SkeShader() {};
	SkeShader(const char* vertexShaderFile, const char* fragmentShaderFile) :Shader(vertexShaderFile, fragmentShaderFile) {};
	~SkeShader() {  };

	virtual void SetupRender(Scene* pScene, Actor* pActor);
	virtual void LinkShader();
};
