#include "pch.h"
#include "ShadowPass.h"

namespace Light
{
	render::ShadowRenderPass::ShadowRenderPass(const std::string & name, IContext * pContext)
	{
	}

	void render::ShadowRenderPass::Render(RenderData & rd)
	{
	}

	void render::ShadowRenderPass::AddRenderObject(Renderable & Obj)
	{
	}

	IActor * render::ShadowRenderPass::RemoveRenderObject(ActorId id)
	{
		return nullptr;
	}
}
