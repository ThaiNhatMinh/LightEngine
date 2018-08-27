#pragma once
#include "Vertex.h"
#include <vector>
namespace Light 
{
	class Mesh
	{
	public:
		virtual ~Mesh()=default;
		virtual void Draw(render::RenderDevice* renderer) = 0;
	};


	class imguiMesh
	{
	private:
		std::vector<DefaultVertex>	m_Vertexs;
		std::vector<unsigned int>	m_Indices;
	public:
		imguiMesh();
		~imguiMesh();

	};

}
