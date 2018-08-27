#pragma once

#include "Mesh.h"

namespace Light
{
	class DefaultMesh : public Mesh
	{
	public:
		typedef std::vector<DefaultVertex> VertexList;
		typedef std::vector<unsigned int> IndicesList;

	private:
		std::string m_Name;
		std::unique_ptr<render::VertexArray>		m_pVAO;
		std::unique_ptr<render::VertexBuffer>		m_pVBO;
		std::unique_ptr<render::IndexBuffer>		m_pIBO;
		std::size_t m_iNNumIndices;

	public:
		DefaultMesh(render::RenderDevice* pRenderDevice,const  VertexList&, const IndicesList&,const std::string& name);
		~DefaultMesh() {};
		virtual void Draw(render::RenderDevice* renderer);
	};
}