#include "pch.h"
#include "DefaultModel.h"
namespace Light
{
	void DefaultModel::Draw(render::RenderDevice* pRenderer,  render::Material::MatrixParam& matrixParam)
	{
		for (std::size_t i = 0; i < Meshs.size(); i++)
		{
			
			Materials[i]->Apply(pRenderer, matrixParam,this->MatParam[i]);
			//pRenderer->SetTexture(render::UNIT_AMBIENT, Ambient[i]);
			pRenderer->SetTexture(render::UNIT_DIFFUSE, Diffuse[i]);
			//pRenderer->SetTexture(render::UNIT_SPECULAR, Specular[i]);
			Meshs[i]->Draw(pRenderer);
		}
	}


	render::Model::MeshList & Light::DefaultModel::GetMeshs()
	{
		return Meshs;
	}
}