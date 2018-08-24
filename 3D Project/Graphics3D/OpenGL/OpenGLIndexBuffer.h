#pragma once
#include "..\Renderer.h"
#include <gl\glew.h>
namespace Light
{
	namespace render
	{
		class OpenGLIndexBuffer :public IndexBuffer
		{
		public:
			OpenGLIndexBuffer(long long size, const void* pData);
			~OpenGLIndexBuffer();
		public:
			GLuint m_iHandle = 0;
		};
	}
}