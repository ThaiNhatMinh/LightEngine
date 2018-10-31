#pragma once
#include <vector>
#include <memory>
#include "..\Graphics3D\Mesh.h"
#include "..\Core\Log.h"
#include "RenderPass.h"
#include "Material.h"
#include "..\Math\AABB.h"
namespace Light
{
	namespace render
	{
		typedef std::vector<std::unique_ptr<Mesh>> MeshList;

		class Model
		{
		public:
			

		public:
			
			virtual ~Model(){};
			virtual void Draw(RenderData& rd,  Material::MatrixParam& matrixParam) = 0;
			virtual MeshList& GetMeshs() = 0;
			virtual math::AABB GetBox() = 0;
		};
	}
}