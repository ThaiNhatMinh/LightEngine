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
			virtual void SetData(long long size, const void* data, BufferUsage usage = STATIC_DRAW)override;
			virtual void* Map(BufferAccess access) override;
			virtual bool UnMap()override;
		public:
			GLuint m_iHandle = 0;
		};
	}
}