#include "pch.h"
#include "DefaultPass.h"
#include "..\..\Interface\IActor.h"
#include "..\..\Math\Math.h"

namespace Light
{
	render::DefaultRenderPass::DefaultRenderPass(const std::string & name, IContext * pContext)
	{
		auto pRS = pContext->GetSystem<IRenderSystem>();
		m_pDebug = pRS->GetDebugRender();
		auto pRenderer = pRS->GetRenderDevice();
		m_name = name;


		pDepthStencilConfig = std::unique_ptr<DepthState>(pRenderer->CreateDepthState());


		param[uMODEL] = nullptr;
		param[uMVP] = nullptr;
		param[uCameraPos] = nullptr;
	}

	void render::DefaultRenderPass::Render(RenderData& rd)
	{
		if (m_ObjectList.size() == 0) return;
		rd.pRenderer->SetDepthState(pDepthStencilConfig.get());
		auto pfrustum = rd.pCamera->GetFrustum();
		
	
		for (Renderable& renderable : m_ObjectList)
		{
			render::Model* modelRender = renderable.m_RenderComponent->m_pModel;
			if (modelRender == nullptr) continue;
			math::AABB box = modelRender->GetBox();
			glm::mat4 t = renderable.m_TransformComponent->GetTransform();
			box = math::TrasformAABB(box, t);

			
			
			if (renderable.pAnimator)
			{
				box = renderable.pAnimator->GetBox();
				box = math::TrasformAABB(box, t);
				if (!pfrustum->Inside(box.Min, box.Max)) continue;
			}else if (!pfrustum->Inside(box.Min, box.Max)) continue;

			//m_pDebug->DrawLineBox(box.Min, box.Max);
			IActor* actor = renderable.m_pActor;
			actor->VPreRender(param);
			// computer transformation matrix
			glm::mat4 model = actor->VGetGlobalTransform();
			param[uMODEL] = glm::value_ptr(model);
			param[uMVP] = glm::value_ptr(rd.pv*model);
			param[uCameraPos] = glm::value_ptr(rd.pCamera->GetPosition());
			// just draw it
			modelRender->Draw(rd,param);
			
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
