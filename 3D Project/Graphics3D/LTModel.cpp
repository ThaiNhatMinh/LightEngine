#include "pch.h"
#include "LTModel.h"

void Light::LTModel::Draw(render::RenderPass * pass, const float * model, const float * mvp)
{
	for (std::size_t i = 0; i<Meshs.size(); i++)
	{
		if(pass->pGlobalMaterial) pass->pGlobalMaterial->Apply(pass->pRenderer, model, mvp);
		else Materials[i]->Apply(pass->pRenderer, model, mvp);
		if (Textures[i]) pass->pRenderer->SetTexture(0, Textures[i]);
		Meshs[i]->Draw(pass->pRenderer);
	}
}
