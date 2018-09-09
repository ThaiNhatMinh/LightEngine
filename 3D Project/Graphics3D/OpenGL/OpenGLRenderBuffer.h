#pragma once

#include "..\Renderer.h"

namespace Light
{
	namespace render
	{
		class OpenGLRenderBuffer : public RenderBuffer
		{
		public:
			GLuint m_iHandle;

		public:
			OpenGLRenderBuffer(int w, int h,InternalFormat format);
			~OpenGLRenderBuffer();

		
		};
	}
}