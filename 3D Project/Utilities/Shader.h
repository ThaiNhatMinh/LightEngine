#pragma once
#include <pch.h>

enum ShaderAttribute
{
	SHADER_POSITION_ATTRIBUTE,
	SHADER_NORMAL_ATTRIBUTE,
	SHADER_TEXCOORD_ATTRIBUTE,
	SHADER_COLOR_ATTRIBUTE,
	SHADER_BLEND1_ATTRIBUTE,
	SHADER_BLEND2_ATTRIBUTE,
	SHADER_BLEND3_ATTRIBUTE,
	SHADER_BLEND4_ATTRIBUTE
};
// base shader class
class Shader: public IShader
{
public:
	GLuint m_iProgramID;
	GLuint m_iVertexShader;
	GLuint m_iFragmentShader;
	map<string, GLuint> m_UniformLists;
	
public:
	Shader() {};
	Shader(const char* vertexShaderFile, const char* fragmentShaderFile);

	virtual ~Shader();
	virtual void LinkShader();
	virtual void SetupRender(Scene*, Actor*) {};

	bool Load(const char* vertexShaderFile, const char* fragmentShaderFile);

	virtual void	VUse() override;
	virtual void	VUnUse() override;
	virtual GLuint	VGetAttribLocation(const char* name) override;;
	virtual GLuint	VGetUniformLocation(const char* name)override;;
	virtual void	VSetUniform(const char* name, float val)override;
	virtual void	VSetUniform(const char* name, float x, float y)override;
	virtual void	VSetUniform(const char* name, float x, float y, float z)override;
	virtual void	VSetUniform(const char* name, const vec3& val)override;
	virtual void	VSetUniform(const char* name, const vec2& val)override;
	virtual void	VSetUniform(const char* name, int val)override;
	virtual void	VSetUniform(const char* name, const glm::mat4& data) override;
	


	
};

