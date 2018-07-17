#pragma once
#include "Vertex.h"
#include <vector>
namespace Light 
{
	class Mesh
	{
	public:
		virtual ~Mesh()=default;
		
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
