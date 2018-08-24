#include "pch.h"
#include "OpenGLPipeline.h"
#include "OpenGLVertexShader.h"
#include "OpenGLPixelShader.h"
#include "OpenGLPipelineParam.h"

using namespace Light;

render::OpenGLPipeline::OpenGLPipeline(VertexShader * pVertexShader, PixelShader * pPixelShader)
{

	static char* attributeName[] = { "aPos", "aNormal", "aUV","aColor","aBlend1","aBlend2","aBlend3","aBlend4" };
	
	
	m_iProgram = glCreateProgram();
	glAttachShader(m_iProgram, static_cast<OpenGLVertexShader*>(pVertexShader)->m_iHandle);
	glAttachShader(m_iProgram, static_cast<OpenGLPixelShader*>(pPixelShader)->m_iHandle);

	// Bind all attribute possible
	for (int i = 0; i < ShaderAttributeLocation::SHADER_NUM; i++)
	{
		glBindAttribLocation(m_iProgram, i, attributeName[i]);
	}


	glLinkProgram(m_iProgram);
	GLint check;
	char infoLog[512];
	
	

	glGetProgramiv(m_iProgram, GL_LINK_STATUS, &check);
	if (check == GL_FALSE)
	{
		glGetProgramInfoLog(m_iProgram, 512, NULL, infoLog);
		E_ERROR("Program Shader link error: %s" ,infoLog);
		return;
	}

	GetUniformList();
}

render::OpenGLPipeline::~OpenGLPipeline()
{
	glDeleteProgram(m_iProgram);
}

render::PipelineParam * render::OpenGLPipeline::GetParam(const char * name)
{
	auto param = m_Uniforms.find(name);
	if (param != m_Uniforms.end()) return param->second.get();

	return nullptr;
}

void render::OpenGLPipeline::GetUniformList()
{
	GLint nUniform, maxLen;
	glGetProgramiv(m_iProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
	glGetProgramiv(m_iProgram, GL_ACTIVE_UNIFORMS, &nUniform);
	GLsizei writen;
	GLenum type;
	GLint size;
	GLint location;
	char name[126];
	for (int i = 0; i < nUniform; i++)
	{

		glGetActiveUniform(m_iProgram, i, maxLen, &writen, &size, &type, name);
		location = glGetUniformLocation(m_iProgram, name);
		m_Uniforms.insert(std::make_pair( string(name),std::unique_ptr<PipelineParam>(new OpenGLPipelineParam(location,name))));

	}
}
