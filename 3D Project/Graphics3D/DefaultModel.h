#pragma once
#include "..\Graphics3D\ModelRender.h"

#include "..\Graphics3D\Material.h"

namespace Light
{
	class render::RenderDevice;

	class DefaultModel : public render::Model
	{
	public:
		math::AABB Box;
		render::MeshList  Meshs;
		//std::vector<render::Texture*> Ambient; 
		std::vector<render::Texture*> Diffuse; // diffuse
		//std::vector<render::Texture*> Specular;
		std::vector<std::shared_ptr<render::Material>>	Materials;
		std::vector<render::MaterialData> MatParam;

		virtual void Draw(render::RenderData& rd,  render::Material::MatrixParam& matrixParam)override;
		virtual render::MeshList& GetMeshs()override;
		virtual math::AABB GetBox()override;
	};
}