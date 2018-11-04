#pragma once
#include "..\Interface\IRenderSystem.h"
#include "Interface\IEvent.h"
#include "..\Interface\IDebugRender.h"
#include "..\Interface\IEffectSystem.h"
namespace Light
{
	namespace render
	{
		class RenderSystem : public IRenderSystem
		{
		public:
			RenderSystem(IContext* pContext);
			virtual void				Update(float dt);
			virtual void				Render();
			virtual render::ICamera*	VGetCurrentCamera()override;
			virtual void				VSetCurrentCamera(render::ICamera * cam) override;
			virtual void				AddExtraPass(RenderPass* pass);
			virtual RenderPass*			GetRenderPass(const std::string& name = "Default")override;
			virtual Texture*			GetSkyBoxTexture();
			virtual IScene*				GetScene();
			const char *				VGetName();
			virtual RenderDevice*		GetRenderDevice() override;
			virtual IDebugRender*		GetDebugRender()override;
			virtual IEffectSystem*		GetEffectSystem()override;
			virtual void				PostInit();
			virtual void				PreRender()override;

			virtual Model*				VCreateModel(resources::ModelData*)override;
			virtual Model*				VCreateModel(std::string xmlfile)override;
			virtual Texture*			VCreateTexture(resources::TextureData*)override;
			virtual Sprite*				VCreateSprite(resources::SpriteData*);
		private:
			//using RenderableList = std::list<Renderable>;
			void LoadRenderDevice();

			void OnObjectCreate(std::shared_ptr<IEvent> event);
			void OnbjectDestroy(std::shared_ptr<IEvent> event);
			void OnCameraCreate(std::shared_ptr<IEvent> event);
			void OnSceneCreate(std::shared_ptr<IEvent> event);
			void AddObjectToPass(IActor* pActor, RenderPass* pass);

		private:
			ICamera * m_pCurrentCamera;
			RenderDevice* m_pRenderer;
			std::unique_ptr<IDebugRender>		m_pDebuger;
			std::unique_ptr<IEffectSystem>		m_pEffectSystem;

			IContext * m_pContext;
			std::unique_ptr<RenderPass> m_DefaultPass;
			std::list<std::unique_ptr<RenderPass>> m_ExtraPass;
			IScene* m_pScene;

		private:
			std::map<resources::TextureData*,std::unique_ptr<Texture>> m_TextureList;
			std::map<resources::ModelData*, std::unique_ptr<Model>> m_ModelList;
			std::map<resources::SpriteData*, std::unique_ptr<Sprite>> m_SpriteList;
		};
	}
}