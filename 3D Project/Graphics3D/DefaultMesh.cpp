#include "pch.h"
#include "DefaultMesh.h"

Light::DefaultMesh::DefaultMesh(render::RenderDevice * pRenderDevice, const VertexList &vertex, const IndicesList &indices, const std::string& name):m_Name(name)
{
	//auto& vertex = pData->Vertexs;
	//auto& indices = pData->Indices;
	m_pVBO = std::unique_ptr<render::VertexBuffer>(pRenderDevice->CreateVertexBuffer(vertex.size() * sizeof(DefaultVertex), &vertex[0]));
	m_pIBO = std::unique_ptr<render::IndexBuffer>(pRenderDevice->CreateIndexBuffer(indices.size() * sizeof(unsigned int), &indices[0]));

	
	size_t stride = sizeof(DefaultVertex);
	render::VertexElement elements[] =
	{
		{ render::SHADER_POSITION_ATTRIBUTE, render::VERTEXELEMENTTYPE_FLOAT, 3, stride, 0 },
		{ render::SHADER_NORMAL_ATTRIBUTE, render::VERTEXELEMENTTYPE_FLOAT, 3, stride, 3 * sizeof(float) },
		{ render::SHADER_TEXCOORD_ATTRIBUTE, render::VERTEXELEMENTTYPE_FLOAT, 2, stride, 6 * sizeof(float) }
	};

	render::VertexDescription* pVertexDes = pRenderDevice->CreateVertexDescription(3, elements);
	render::VertexBuffer* ptemp = m_pVBO.get();
	
	m_pVAO = std::unique_ptr<render::VertexArray>(pRenderDevice->CreateVertexArray(1, &ptemp, &pVertexDes));
	
	m_iNNumIndices = indices.size();
	
	delete pVertexDes;
}

void Light::DefaultMesh::Draw(render::RenderDevice * renderer)
{
	
	renderer->SetVertexArray(m_pVAO.get());
	renderer->SetIndexBuffer(m_pIBO.get());
	renderer->DrawElement(m_iNNumIndices, 0);
}
