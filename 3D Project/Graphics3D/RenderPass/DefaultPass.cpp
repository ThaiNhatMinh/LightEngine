#include "pch.h"
#include "DefaultPass.h"
#include "..\..\Interface\IActor.h"
namespace Light
{
	render::DefaultRenderPass::DefaultRenderPass(const std::string & name, IContext * pContext)
	{
		auto pRS = pContext->GetSystem<IRenderSystem>();
		auto pRenderer = pRS->GetRenderDevice();
		m_name = name;


		pDepthStencilConfig = std::unique_ptr<DepthState>(pRenderer->CreateDepthState());


		param[uMODEL] = nullptr;
		param[uMVP] = nullptr;
		param[uCameraPos] = nullptr;
	}

	void render::DefaultRenderPass::Render(const glm::mat4& pv, RenderDevice* pRenderer, ICamera* pCamera)
	{
		if (m_ObjectList.size() == 0) return;
		pRenderer->SetDepthState(pDepthStencilConfig.get());
	
		
		for (Renderable& renderable : m_ObjectList)
		{
			render::Model* modelRender = renderable.m_RenderComponent->m_pModel;
			IActor* actor = renderable.m_pActor;
			actor->VPreRender(param);
			// computer transformation matrix
			glm::mat4 model = actor->VGetGlobalTransform();
			param[uMODEL] = glm::value_ptr(model);
			param[uMVP] = glm::value_ptr(pv*model);
			param[uCameraPos] = glm::value_ptr(pCamera->GetPosition());
			// just draw it
			modelRender->Draw(pRenderer, param);
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
