#include "pch.h"
#include "DefaultModel.h"
namespace Light
{
	void DefaultModel::Draw(render::RenderDevice* pRenderer, const float * model, const float * mvp)
	{
		for (std::size_t i = 0; i < Meshs.size(); i++)
		{
			
			Materials[i]->Apply(pRenderer, model, mvp);
			pRenderer->SetTexture(render::UNIT_DEFAULT, Textures[i]);
			Meshs[i]->Draw(pRenderer);
		}
	}

	render::Model::MeshList & Light::DefaultModel::GetMeshs()
	{
		return Meshs;
	}
}