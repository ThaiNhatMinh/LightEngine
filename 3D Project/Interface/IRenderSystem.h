#pragma once

#include "ICamera.h"
#include "..\Graphics3D\RenderPass.h"
#include "IContext.h"
#include "IEffectSystem.h"
#include "IDebugRender.h"
namespace Light
{
	namespace render
	{
		class IRenderSystem : public ISubSystem
		{
		public:
			virtual render::ICamera*	VGetCurrentCamera() = 0;
			virtual void				VSetCurrentCamera(render::ICamera * cam) = 0;

			virtual void				AddExtraPass(RenderPass* pass) = 0;
			virtual RenderPass*			GetRenderPass(const std::string& name = "Default") = 0;

			virtual Texture*			GetSkyBoxTexture() = 0;
			virtual IScene*				GetScene() = 0;
			virtual RenderDevice*		GetRenderDevice() = 0;
			virtual IDebugRender*		GetDebugRender()=0;
			virtual IEffectSystem*		GetEffectSystem()=0;

			// create render data
			virtual Model*				VCreateModel(resources::ModelData*) = 0;
			virtual Model*				VCreateModel(std::string xmlfile) = 0;
			virtual Texture*			VCreateTexture(resources::TextureData*) = 0;
			virtual Sprite*				VCreateSprite(resources::SpriteData*) = 0;
		};
	}
}