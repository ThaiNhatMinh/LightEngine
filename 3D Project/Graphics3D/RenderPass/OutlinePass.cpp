#include "pch.h"
#include "OutlinePass.h"
#include "..\..\Interface\IFactory.h"

namespace Light
{
	render::OutlineRenderPass::OutlineRenderPass(const std::string & name, IContext * pContext)
	{
		m_name = name;
		pRenderer = pContext->GetSystem<RenderDevice>();


		{
			render::DepthStencilConfig config;
			config.FrontStencilEnabled = true;
			config.FrontWriteMask = 0xFFFFFFFF;
			config.FrontStencilCompare = render::COMPARE_ALWAYS;
			config.FrontRef = 1;
			config.FrontCompareMask = 0xFFFFFFFF;
			config.FrontDepthFail = STENCIL_REPLACE;
			pDepthStencilConfig1 = std::unique_ptr<DepthStencilState>(pRenderer->CreateDepthStencilState(config));
		}

		{
			render::DepthStencilConfig config;
			config.FrontStencilEnabled = true;
			config.FrontStencilCompare = render::COMPARE_NOTEQUAL;
			config.FrontRef = 1;
			config.FrontCompareMask = 0xFFFFFFFF;
			//config.FrontWriteMask = 0x00;
			//config.FrontStencilPass = STENCIL_KEEP;
			
			config.DepthEnable = false;
			pDepthStencilConfig2 = std::unique_ptr<DepthStencilState>(pRenderer->CreateDepthStencilState(config));
			
			auto Resources = pContext->GetSystem<resources::IResourceManager>();
			auto VS = Resources->VGetVertexShader("Outline");
			auto FS = Resources->VGetPixelShader("Color");
			auto pipeline = pRenderer->CreatePipeline(VS, FS);

			pGlobalMaterial = pContext->GetSystem<IFactory>()->VGetMaterial("Default")->Clone();
			pGlobalMaterial->SetPipeline(pipeline);
		}

		{
			render::DepthStencilConfig config;
			config.DepthEnable = true;
			config.Depthfunc = COMPARE_LESS;
			config.DepthMask = true;

			config.FrontStencilEnabled = true;
			config.FrontWriteMask = 0xFF;
			config.FrontStencilCompare = COMPARE_NOTEQUAL;
			config.FrontRef = 1;
			config.FrontCompareMask = 0xFF;
			config.FrontStencilFail = STENCIL_KEEP;
			config.FrontDepthFail = STENCIL_KEEP;
			config.FrontStencilPass = STENCIL_KEEP;
			pDepthStencilConfig3 = std::unique_ptr<DepthStencilState>(pRenderer->CreateDepthStencilState(config));

		}
		
	}

	void render::OutlineRenderPass::Render(const glm::mat4 & pv)
	{
		if (m_ObjectList.size() == 0) return;

		pRenderer->SetDepthStencilState(pDepthStencilConfig1.get());

		Material::MatrixParam param;

		for (Renderable& renderable : m_ObjectList)
		{
			render::Model* modelRender = renderable.m_RenderComponent->m_pModel;
			IActor* actor = renderable.m_pActor;
			// computer transformation matrix
			glm::mat4 model = actor->VGetGlobalTransform();

			param[uMODEL] = glm::value_ptr(model);
			param[uMVP] = glm::value_ptr(pv*model);
			// just draw it
			modelRender->Draw(pRenderer, param);
		}

		pRenderer->SetDepthStencilState(pDepthStencilConfig2.get());
		for (Renderable& renderable : m_ObjectList)
		{
			render::Model* modelRender = renderable.m_RenderComponent->m_pModel;
			IActor* actor = renderable.m_pActor;
			// computer transformation matrix
			glm::mat4 model = actor->VGetGlobalTransform();

			param[uMODEL] = glm::value_ptr(model);
			param[uMVP] = glm::value_ptr(pv*model);

			pGlobalMaterial->Apply(pRenderer, param);

			// just draw it
			Model::MeshList& meshs = modelRender->GetMeshs();
			for (auto& mesh : meshs)
				mesh->Draw(pRenderer);
		}
		pRenderer->SetDepthStencilState(pDepthStencilConfig3.get());
	}

	void render::OutlineRenderPass::AddRenderObject(Renderable & Obj)
	{
		RenderPass* pDefaultPass = pRenderer->GetRenderPass();
		if (Obj.m_pActor->VGetName() != "707_No")
		{
			IActor* pActor = pDefaultPass->RemoveRenderObject(Obj.m_ActorID);
			m_ObjectList.push_back(Obj);
		}

		

	}

	IActor * render::OutlineRenderPass::RemoveRenderObject(ActorId id)
	{
		auto r = std::find_if(m_ObjectList.begin(), m_ObjectList.end(), [id](const Renderable& renderable)
		{
			return id == renderable.m_ActorID;
		});

		if (r == m_ObjectList.end()) return nullptr;

		return (*r).m_pActor;
	}
}