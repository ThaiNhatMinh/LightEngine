#pragma once

#include "..\pch.h"

// Shader with texture and lighting
class TexShader : public Shader
{

public:
	TexShader() {};
	TexShader(const char* vertexShaderFile, const char* fragmentShaderFile) :Shader(vertexShaderFile, fragmentShaderFile) {};
	~TexShader() {  };

	virtual void SetupRender(Scene* pScene, Actor* pActor);

};
