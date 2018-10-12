#include "pch.h"
#include "LTModel.h"

namespace Light
{
	void LTModel::Draw(render::RenderDevice* pRenderer,  render::Material::MatrixParam& matrixParam)
	{

		for (std::size_t i = 0; i < Meshs.size(); i++)
		{
			
			Materials[i]->Apply(pRenderer,matrixParam);
			pRenderer->SetTexture(render::UNIT_DIFFUSE, Textures[i]);
			Meshs[i]->Draw(pRenderer);
		}
	}


	render::Model::MeshList & LTModel::GetMeshs()
	{
		return Meshs;
	}
	math::AABB LTModel::GetBox()
	{
		return math::AABB();
	}
}