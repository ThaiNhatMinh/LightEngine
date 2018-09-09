#include "pch.h"
#include "OpenGLVertexDescription.h"
#include "DataMap.h"
using namespace Light;

render::OpenGLVertexDescription::OpenGLVertexDescription(std::size_t num, const VertexElement * pElements) :m_Elements(DEBUG_NEW OpenGLVertexElement[num]), m_iNum(num)
{
	

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
