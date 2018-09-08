#pragma once
#include <vector>
#include <memory>
#include "..\Graphics3D\Mesh.h"
#include "..\Core\Log.h"
#include "RenderPass.h"

namespace Light
{
	namespace render
	{
		
		class Model
		{
		public:
			typedef std::vector<std::unique_ptr<Mesh>> MeshList;

		public:
			virtual ~Model(){};
			virtual void Draw(render::RenderDevice* pRenderer, const float * model, const float * mvp) = 0;
			virtual MeshList& GetMeshs() = 0;
		};
	}
}