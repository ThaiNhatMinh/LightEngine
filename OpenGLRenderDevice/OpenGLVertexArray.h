#pragma once

#include <Renderer.h>
#include <glew.h>
namespace Light {
	namespace render
	{
		class OpenGLVertexArray : public VertexArray
		{
		public:
			OpenGLVertexArray(unsigned int numBuffer, VertexBuffer**vertexBuffer, VertexDescription** vertexDescription);
			~OpenGLVertexArray();


		public:
			GLuint m_iHandle = 0;
		};
	}
}