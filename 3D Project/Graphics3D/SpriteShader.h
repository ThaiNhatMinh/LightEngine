#pragma once


class SpriteShader : public Shader
{
public:

	SpriteShader() {};
	SpriteShader(const char* vertexShaderFile, const char* fragmentShaderFile) :Shader(vertexShaderFile, fragmentShaderFile) {};
	~SpriteShader() {};

	
	virtual void LinkShader();
};