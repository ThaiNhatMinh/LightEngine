#include "pch.h"
#include "LTModel.h"

namespace Light
{
	void LTModel::Draw(render::RenderDevice* pRenderer, const float * model, const float * mvp)
	{
		for (std::size_t i = 0; i < Meshs.size(); i++)
		{
			//if (pass->pGlobalMaterial) pass->pGlobalMaterial->Apply(pass->pRenderer, model, mvp);
			//else
			Materials[i]->Apply(pRenderer, model, mvp);
			pRenderer->SetTexture(0, Textures[i]);
			Meshs[i]->Draw(pRenderer);
		}
	}

	render::Model::MeshList & LTModel::GetMeshs()
	{
		return Meshs;
	}
}