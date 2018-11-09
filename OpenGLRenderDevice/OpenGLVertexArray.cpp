#include "stdafx.h"
#include "OpenGLVertexArray.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLVertexDescription.h"
using namespace Light;
render::OpenGLVertexArray::OpenGLVertexArray(unsigned int numBuffer, VertexBuffer ** vertexBuffers, VertexDescription ** vertexDescription)
{
	glGenVertexArrays(1, &m_iHandle);
	glBindVertexArray(m_iHandle);

	for (std::size_t i = 0; i < numBuffer; i++)
	{
		
		OpenGLVertexBuffer* vertexBuffer = static_cast<OpenGLVertexBuffer*>(vertexBuffers[i]);
		OpenGLVertexDescription* vertexDes = static_cast<OpenGLVertexDescription*>(vertexDescription[i]);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_iHandle);	// GL_ARRAY_BUFFER for now, update late
		for (size_t j = 0; j < vertexDes->m_iNum; j++)
		{
			glEnableVertexAttribArray(vertexDes->m_Elements[j].index);
			glVertexAttribPointer(vertexDes->m_Elements[j].index, vertexDes->m_Elements[j].size, vertexDes->m_Elements[j].type,
				vertexDes->m_Elements[j].normalized, vertexDes->m_Elements[j].stride, vertexDes->m_Elements[j].pointer);
		}
	} 
	glBindVertexArray(0);
}

render::OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &m_iHandle);
}
