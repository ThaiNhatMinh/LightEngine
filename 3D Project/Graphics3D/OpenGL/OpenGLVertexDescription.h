#pragma once

#include "..\Renderer.h"
#include <gl\glew.h>
namespace Light
{
	namespace render
	{
		class OpenGLVertexDescription : public VertexDescription
		{
		public:
			struct OpenGLVertexElement
			{
				GLuint index;
				GLint size;
				GLenum type;
				GLboolean normalized;
				GLsizei stride;
				const GLvoid *pointer;
			};
			OpenGLVertexDescription(std::size_t num, const VertexElement* pElements);
			~OpenGLVertexDescription();

		public:
			std::unique_ptr<OpenGLVertexElement[]> m_Elements;
			std::size_t m_iNum;
		};
	}
}