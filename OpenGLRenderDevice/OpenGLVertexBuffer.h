#pragma once

#include <Renderer.h>
#include <glew.h>
namespace Light {
	namespace render
	{
		class OpenGLVertexBuffer : public VertexBuffer
		{
		public:
			OpenGLVertexBuffer(long long size, const void* data);
			~OpenGLVertexBuffer();

			virtual void SetData(long long size, const void* data, BufferUsage usage = STATIC_DRAW)override;
			virtual void* Map(BufferAccess access) override;
			virtual bool UnMap()override;
		public:
			GLuint m_iHandle = 0;
		};
	}
}