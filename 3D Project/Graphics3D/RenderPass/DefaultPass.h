#pragma once

#include "..\..\Interface\IRenderSystem.h"
#include "..\RenderPass.h"
#include "..\Material.h"
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
		public:
			DefaultRenderPass(const std::string& name, IContext* pContext);
			virtual void Render(const glm::mat4& pv, RenderDevice* pRenderer, ICamera* pCamera)override;
			virtual void AddRenderObject(Renderable& Obj)override;
			virtual IActor* RemoveRenderObject(ActorId id)override;
		};
	}
}