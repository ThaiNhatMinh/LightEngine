#pragma once


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
			std::unique_ptr<DepthStencilState> pDepthStencilConfig;
			Material::MatrixParam param;
		public:
			DefaultRenderPass(const std::string& name, IContext* pContext);
			virtual void Render(const glm::mat4& pv)override;
			virtual void AddRenderObject(Renderable& Obj)override;
			virtual IActor* RemoveRenderObject(ActorId id)override;
		};
	}
}