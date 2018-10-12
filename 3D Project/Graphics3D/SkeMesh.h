#pragma once
#include <vector>
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>
#include "Mesh.h"
#include "Vertex.h"
#include "..\ResourceManager\LTRawData.h"

namespace Light
{	
	class SkeMesh :public Mesh
	{
	public:
		typedef std::vector<SkeVertex> VertexList;
		typedef std::vector<unsigned int> IndicesList;
	private:
		std::string m_Name;
		std::unique_ptr<render::VertexArray>		m_pVAO;
		std::unique_ptr<render::VertexBuffer>		m_pVBO;
		std::unique_ptr<render::IndexBuffer>		m_pIBO;
		std::size_t m_iNNumIndices;
	public:
		SkeMesh(render::RenderDevice* pRenderDevice, LTRawMesh* pData);
		~SkeMesh() {};
		virtual void Draw(render::RenderDevice * renderer)override;
	};
}