#include "pch.h"
#include "DefaultModel.h"
namespace Light
{
	void DefaultModel::Draw(render::RenderDevice* pRenderer, const float * model, const float * mvp)
	{
		for (std::size_t i = 0; i < Meshs.size(); i++)
		{
			//if (pass->pGlobalMaterial) pass->pGlobalMaterial->Apply(pass->pRenderer, model, mvp);
			//else
			Materials[i]->Apply(pRenderer, model, mvp);
			if (Textures[i]) pRenderer->SetTexture(0, Textures[i]);
			Meshs[i]->Draw(pRenderer);
		}
	}

	render::Model::MeshList & Light::DefaultModel::GetMeshs()
	{
		return Meshs;
	}
}