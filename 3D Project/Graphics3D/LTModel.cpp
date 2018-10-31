#include "pch.h"
#include "LTModel.h"

namespace Light
{
	void LTModel::Draw(render::RenderData& rd,  render::Material::MatrixParam& matrixParam)
	{

		for (std::size_t i = 0; i < Meshs.size(); i++)
		{
			
			Materials[i]->Apply(rd.pRenderer,matrixParam);
			rd.pRenderer->SetTexture(render::UNIT_DIFFUSE, Textures[i]);
			Meshs[i]->Draw(rd.pRenderer);
		}
	}


	render::MeshList & LTModel::GetMeshs()
	{
		return Meshs;
	}
	math::AABB LTModel::GetBox()
	{
		return math::AABB();
	}
}