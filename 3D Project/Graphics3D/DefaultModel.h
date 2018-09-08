#pragma once
#include "..\Graphics3D\ModelRender.h"

#include "..\Graphics3D\Material.h"

namespace Light
{
	class render::RenderDevice;

	class DefaultModel : public render::Model
	{
	public:
		MeshList  Meshs;
		std::vector<render::Texture*> Textures;
		std::vector<std::shared_ptr<render::Material>>	Materials;

		virtual void Draw(render::RenderDevice* pRenderer, const float * model, const float * mvp);
		virtual MeshList& GetMeshs()override;
	};
}