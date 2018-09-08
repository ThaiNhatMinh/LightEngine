#include "pch.h"
#include "DefaultPass.h"
namespace Light
{
	render::DefaultRenderPass::DefaultRenderPass(const std::string & name, IContext * pContext)
	{
		pRenderer = pContext->GetSystem<RenderDevice>();
		m_name = name;


		render::DepthStencilConfig config;
		config.DepthEnable = true;
		config.Depthfunc = COMPARE_LESS;
		config.DepthMask = true;

		config.FrontStencilEnabled = true;
		config.FrontWriteMask = 0x00;
		config.FrontStencilCompare = COMPARE_NOTEQUAL;
		config.FrontRef = 1;
		config.FrontCompareMask = 0xFF;
		config.FrontStencilFail = STENCIL_KEEP;
		config.FrontDepthFail = STENCIL_KEEP;
		config.FrontStencilPass = STENCIL_KEEP;

		pDepthStencilConfig = std::unique_ptr<DepthStencilState>(pRenderer->CreateDepthStencilState(config));

	}

	void render::DefaultRenderPass::Render(const glm::mat4 & pv)
	{
		if (m_ObjectList.size() == 0) return;
		pRenderer->SetDepthStencilState(pDepthStencilConfig.get());
	
		for (Renderable& renderable : m_ObjectList)
		{
			render::Model* modelRender = renderable.m_RenderComponent->m_pModel;
			IActor* actor = renderable.m_pActor;
			// computer transformation matrix
			glm::mat4 model = actor->VGetGlobalTransform();
			// just draw it
			modelRender->Draw(pRenderer, glm::value_ptr(model), glm::value_ptr(pv*model));
		}
	}

	void render::DefaultRenderPass::AddRenderObject(Renderable & Obj)
	{
		// doesn't check anything here
		// RenderDevice already do it
		m_ObjectList.push_back(Obj);
	}

	IActor* render::DefaultRenderPass::RemoveRenderObject(ActorId id)
	{
		auto result = std::find_if(m_ObjectList.begin(), m_ObjectList.end(), [id](Renderable renderable)
		{
			return id == renderable.m_ActorID;
		});

		if (result == m_ObjectList.end()) return nullptr;

		IActor* pActor = result->m_pActor;

		m_ObjectList.erase(result);

		return pActor;
	}
}
