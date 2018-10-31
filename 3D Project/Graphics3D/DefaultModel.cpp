#include "pch.h"
#include "DefaultModel.h"
namespace Light
{
	void DefaultModel::Draw(render::RenderData& rd,  render::Material::MatrixParam& matrixParam)
	{
		for (std::size_t i = 0; i < Meshs.size(); i++)
		{
			
			Materials[i]->Apply(rd.pRenderer, matrixParam,this->MatParam[i]);
			//pRenderer->SetTexture(render::UNIT_AMBIENT, Ambient[i]);
			rd.pRenderer->SetTexture(render::UNIT_DIFFUSE, Diffuse[i]);
			//pRenderer->SetTexture(render::UNIT_SPECULAR, Specular[i]);
			Meshs[i]->Draw(rd.pRenderer);
		}
	}


	render::MeshList & Light::DefaultModel::GetMeshs()
	{
		return Meshs;
	}
	math::AABB DefaultModel::GetBox()
	{
		return Box;
	}
}