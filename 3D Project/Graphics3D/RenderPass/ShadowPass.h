#pragma once
#include "..\..\Interface\IRenderSystem.h"
#include "..\RenderPass.h"

namespace Light
{
	namespace render
	{
		class ShadowRenderPass : public RenderPass
		{
		private:
			IRenderSystem* m_pRS;
			std::shared_ptr<Material> pGlobalMaterial = nullptr;
			std::unique_ptr<StencilState> pStencilConfig1;
			std::unique_ptr<StencilState> pStencilConfig2;
			std::unique_ptr<DepthState> pDepthConfig2;
			std::unique_ptr<StencilState> pStencilConfig3;
			std::unique_ptr<DepthState> pDepthConfig3;
		public:
			ShadowRenderPass(const std::string& name, IContext* pContext);
			virtual void Render(RenderData& rd)override;
			virtual void AddRenderObject(Renderable& Obj)override;
			virtual IActor* RemoveRenderObject(ActorId id)override;
		};
	}
}