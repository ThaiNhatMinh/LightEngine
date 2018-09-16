#pragma once

#include "..\Renderer.h"
#include "..\Material.h"

namespace Light
{
	namespace render
	{
		class OutlineRenderPass : public RenderPass
		{
		private:

			std::shared_ptr<Material> pGlobalMaterial = nullptr;
			std::unique_ptr<DepthStencilState> pDepthStencilConfig1;
			std::unique_ptr<DepthStencilState> pDepthStencilConfig2;
			std::unique_ptr<DepthStencilState> pDepthStencilConfig3;
		public:
			OutlineRenderPass(const std::string& name, IContext* pContext);
			virtual void Render(const glm::mat4& pv)override;
			virtual void AddRenderObject(Renderable& Obj)override;
			virtual IActor* RemoveRenderObject(ActorId id)override;
		};
	}
}