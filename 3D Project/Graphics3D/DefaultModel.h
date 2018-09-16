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
		//std::vector<render::Texture*> Ambient; 
		std::vector<render::Texture*> Diffuse; // diffuse
		//std::vector<render::Texture*> Specular;
		std::vector<std::shared_ptr<render::Material>>	Materials;
		std::vector<render::MaterialData> MatParam;

		virtual void Draw(render::RenderDevice* pRenderer, const float * model, const float * mvp);
		virtual MeshList& GetMeshs()override;
	};
}