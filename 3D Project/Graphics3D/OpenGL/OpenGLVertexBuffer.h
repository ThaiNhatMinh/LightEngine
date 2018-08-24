#pragma once

#include "..\Renderer.h"
#include <gl\glew.h>
namespace Light {
	namespace render
	{
		class OpenGLVertexBuffer : public VertexBuffer
		{
		public:
			OpenGLVertexBuffer(long long size, const void* data);
			~OpenGLVertexBuffer();
		public:
			GLuint m_iHandle = 0;
		};
	}
}