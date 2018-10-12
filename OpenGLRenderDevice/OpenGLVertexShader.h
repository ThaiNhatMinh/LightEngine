#pragma once


#include <glew.h>
namespace Light
{
	namespace render
	{
		class OpenGLVertexShader : public VertexShader
		{
		public:
			OpenGLVertexShader(const char* code);
			~OpenGLVertexShader();
		public:
			GLuint m_iHandle;
		};
	}
}