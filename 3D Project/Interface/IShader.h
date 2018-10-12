#pragma once


//class IShader
//{
//public:
//	IShader() = default;
//	virtual void	VUse() = 0;
//	virtual void	VUnUse() = 0;
//	virtual GLuint	VGetAttribLocation(const char* name)=0;
//	virtual GLuint	VGetUniformLocation(const char* name)=0;
//
//	virtual void	VSetUniform(const char* name, float val) = 0;
//	virtual void	VSetUniform(const char* name, float x, float y) = 0;
//	virtual void	VSetUniform(const char* name, float x, float y, float z) = 0;
//	virtual void	VSetUniform(const char* name, const vec3& val) = 0;
//	virtual void	VSetUniform(const char* name, const vec2& val) = 0;
//	virtual void	VSetUniform(const char* name, int val) = 0;
//	virtual void	VSetUniform(const char* name, const glm::mat4& m) = 0;
//private:
//	IShader(const IShader& other) = delete;
//	IShader(IShader&& other) = delete;
//	IShader& operator=(const IShader& other) = delete;
//	IShader& operator=(IShader&& other) = delete;
//};