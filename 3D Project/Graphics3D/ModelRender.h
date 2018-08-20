#pragma once
#include <vector>
#include <memory>
#include "Mesh.h"
#include "Material.h"

class Texture;
namespace Light
{
	namespace render
	{
		class Model
		{
		public:
			virtual ~Model() = default;
		};

		class ModelRender
		{
		public:
			ModelRender() {};
		public:

			std::vector<std::unique_ptr<Mesh>>  m_pMesh;
			std::vector<Texture*> m_Textures;
			std::vector<std::unique_ptr<Material*>>	m_Material;
		};
	}
}