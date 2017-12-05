#pragma once


class DebugShader : public Shader
{
public:
	DebugShader() {};
	DebugShader(const char* vertexShaderFile, const char* fragmentShaderFile) :Shader(vertexShaderFile, fragmentShaderFile) {};
	~DebugShader() {  };


	virtual void LinkShader();
};