#pragma once
#include "Mesh.h"
#include "Material.h"
#include "..\ResourceManager\LTRawData.h"
#include "Renderer.h"
namespace Light
{
	namespace render
	{
		class ModelRender
		{
		public:
			ModelRender() {};
		public:

			std::vector<std::unique_ptr<Mesh>>  m_pMesh;
			std::vector<Material>	m_Material;
		};
	}
}