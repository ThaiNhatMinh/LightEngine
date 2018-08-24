#include "pch.h"
#include "OpenGLVertexDescription.h"

using namespace Light;

render::OpenGLVertexDescription::OpenGLVertexDescription(std::size_t num, const VertexElement * pElements) :m_Elements(new OpenGLVertexElement[num]), m_iNum(num)
{
	static GLenum toOpenGLType[] = { 
		GL_BYTE, GL_SHORT, GL_INT, 
		GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT,
		GL_BYTE, GL_SHORT, GL_INT, 
		GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT, 
		GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE };

	static GLboolean toOpenGLNormalized[] = { 
		GL_FALSE, GL_FALSE, GL_FALSE, 
		GL_FALSE, GL_FALSE, GL_FALSE,
		GL_TRUE, GL_TRUE, GL_TRUE, 
		GL_TRUE, GL_TRUE, GL_TRUE, 
		GL_FALSE, GL_FALSE, GL_FALSE };

	for (std::size_t i = 0; i < num; i++)
	{
		m_Elements[i].index = pElements[i].index;
		m_Elements[i].size = pElements[i].size;
		m_Elements[i].stride = pElements[i].stride;
		m_Elements[i].normalized = toOpenGLNormalized[pElements[i].type];
		m_Elements[i].pointer = (char*)nullptr + pElements[i].offset;
		m_Elements[i].type = toOpenGLType[pElements[i].type];
	}

	
}

render::OpenGLVertexDescription::~OpenGLVertexDescription()
{
}
