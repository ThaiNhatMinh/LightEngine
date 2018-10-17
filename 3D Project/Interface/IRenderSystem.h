#pragma once

#include "ICamera.h"
#include "..\Graphics3D\RenderPass.h"
#include "IContext.h"
#include "IEffectSystem.h"
namespace Light
{
	namespace render
	{
		class IRenderSystem : public ISubSystem
		{
		public:
			virtual void				Update(float dt) = 0;
			virtual void				Render() = 0;
			virtual render::ICamera*	VGetCurrentCamera() = 0;
			virtual void				VSetCurrentCamera(render::ICamera * cam) = 0;

			virtual void				AddExtraPass(RenderPass* pass) = 0;
			virtual RenderPass*			GetRenderPass(const std::string& name = "Default") = 0;

			virtual Texture*			GetSkyBoxTexture() = 0;
			virtual IScene*				GetScene() = 0;
			virtual RenderDevice*		GetRenderDevice() = 0;
			virtual IDebugRender*		GetDebugRender()=0;
			virtual IEffectSystem*		GetEffectSystem()=0;
		};
	}
}