#include "pch.h"
#include "OutlinePass.h"
#include "..\..\Interface\IFactory.h"

namespace Light
{
	render::OutlineRenderPass::OutlineRenderPass(const std::string & name, IContext * pContext)
	{
		m_name = name;
		m_pRS = pContext->GetSystem<IRenderSystem>();
		auto pRenderer = m_pRS->GetRenderDevice();


		{
			render::StencilConfig config;
			config.FrontEnabled = true;
			config.FrontWriteMask = 0xFFFFFFFF;
			config.FrontCompare = render::COMPARE_ALWAYS;
			config.FrontRef = 1;
			config.FrontCompareMask = 0xFFFFFFFF;
			config.FrontDepthFail = STENCIL_REPLACE;
			pStencilConfig1 = std::unique_ptr<StencilState>(pRenderer->CreateStencilState(config));
		}

		{
			render::StencilConfig config;
			config.FrontEnabled = true;
			config.FrontCompare = render::COMPARE_NOTEQUAL;
			config.FrontRef = 1;
			config.FrontCompareMask = 0xFFFFFFFF;
			//config.FrontWriteMask = 0x00;
			//config.FrontStencilPass = STENCIL_KEEP;
			
			
			pStencilConfig2 = std::unique_ptr<StencilState>(pRenderer->CreateStencilState(config));
			pDepthConfig2 = std::unique_ptr<DepthState>(pRenderer->CreateDepthState(false));

			auto Resources = pContext->GetSystem<resources::IResourceManager>();
			auto VS = Resources->VGetVertexShader("Outline");
			auto FS = Resources->VGetPixelShader("Color");
			auto pipeline = pRenderer->CreatePipeline(VS, FS);

			pGlobalMaterial = pContext->GetSystem<IFactory>()->VGetMaterial("Default")->Clone();
			pGlobalMaterial->SetPipeline(pipeline);
		}

		{
			render::StencilConfig config;

			config.FrontEnabled = true;
			config.FrontWriteMask = 0xFF;
			config.FrontCompare = COMPARE_NOTEQUAL;
			config.FrontRef = 1;
			config.FrontCompareMask = 0xFF;
			config.FrontStencilFail = STENCIL_KEEP;
			config.FrontDepthFail = STENCIL_KEEP;
			config.FrontPass = STENCIL_KEEP;
			pStencilConfig3 = std::unique_ptr<StencilState>(pRenderer->CreateStencilState(config));
			pDepthConfig3 = std::unique_ptr<DepthState>(pRenderer->CreateDepthState());
		}
		
	}

	void render::OutlineRenderPass::Render(RenderData& rd)
	{
		if (m_ObjectList.size() == 0) return;

		rd.pRenderer->SetStencilState(pStencilConfig1.get());

		Material::MatrixParam param;

		for (Renderable& renderable : m_ObjectList)
		{
			render::Model* modelRender = renderable.m_RenderComponent->m_pModel;
			IActor* actor = renderable.m_pActor;
			// computer transformation matrix
			glm::mat4 model = actor->VGetGlobalTransform();

			param[uMODEL] = glm::value_ptr(model);
			param[uMVP] = glm::value_ptr(rd.pv*model);
			// just draw it
			modelRender->Draw(rd, param);
		}

		rd.pRenderer->SetStencilState(pStencilConfig2.get());
		rd.pRenderer->SetDepthState(pDepthConfig2.get());

		for (Renderable& renderable : m_ObjectList)
		{
			render::Model* modelRender = renderable.m_RenderComponent->m_pModel;
			IActor* actor = renderable.m_pActor;
			// computer transformation matrix
			glm::mat4 model = actor->VGetGlobalTransform();

			param[uMODEL] = glm::value_ptr(model);
			param[uMVP] = glm::value_ptr(rd.pv*model);

			pGlobalMaterial->Apply(rd.pRenderer, param);

			// just draw it
			MeshList& meshs = modelRender->GetMeshs();
			for (auto& mesh : meshs)
				mesh->Draw(rd.pRenderer);
		}
		rd.pRenderer->SetStencilState(pStencilConfig3.get());
		rd.pRenderer->SetDepthState(pDepthConfig3.get());
	}

	void render::OutlineRenderPass::AddRenderObject(Renderable & Obj)
	{
		RenderPass* pDefaultPass = m_pRS->GetRenderPass();
		//if (Obj.m_pActor->VGetName() != "707_No")
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