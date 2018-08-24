#pragma once
#include "..\Renderer.h"
#include <gl\glew.h>

namespace Light {

	namespace render
	{
		class OpenGLPixelShader : public PixelShader
		{
		public:
			OpenGLPixelShader(const char* code);
			~OpenGLPixelShader();
		public:
			GLuint m_iHandle;
		};
	}
}