#pragma once

#include "pch.h"

// Shader with texture and lighting
class PrimShader : public Shader
{

public:
	PrimShader() {};
	virtual ~PrimShader() { };
	PrimShader(const char* vertexShaderFile, const char* fragmentShaderFile) :Shader(vertexShaderFile,fragmentShaderFile) {};

	virtual void SetupRender(Scene* pScene, Actor* pActor);
	virtual void LinkShader();
};
