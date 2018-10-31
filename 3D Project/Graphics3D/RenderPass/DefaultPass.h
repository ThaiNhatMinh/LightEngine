#pragma once

#include "..\..\Interface\IRenderSystem.h"
#include "..\RenderPass.h"
#include "..\Material.h"
#include "..\..\Interface\IDebugRender.h"
namespace Light
{
	namespace render
	{
		// default render pass
		// using obj config
		class DefaultRenderPass : public RenderPass
		{
		private:

			std::shared_ptr<Material> pGlobalMaterial = nullptr;
			std::unique_ptr<DepthState> pDepthStencilConfig;
			Material::MatrixParam param;
			IRenderSystem* m_pRS;
			IDebugRender* m_pDebug;
		public:
			DefaultRenderPass(const std::string& name, IContext* pContext);
			virtual void Render(RenderData& rd)override;
			virtual void AddRenderObject(Renderable& Obj)override;
			virtual IActor* RemoveRenderObject(ActorId id)override;
		};
	}
}