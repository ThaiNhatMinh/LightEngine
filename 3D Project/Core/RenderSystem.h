#pragma once
#include "..\Interface\IRenderSystem.h"
#include "Interface\IEvent.h"
namespace Light
{
	namespace render
	{
		class RenderSystem : public IRenderSystem
		{
		public:
			RenderSystem(IContext* pContext);
			virtual void				Render();
			virtual render::ICamera*	VGetCurrentCamera()override;
			virtual void				VSetCurrentCamera(render::ICamera * cam) override;
			virtual void				AddExtraPass(RenderPass* pass);
			virtual RenderPass*			GetRenderPass(const std::string& name = "Default")override;
			virtual Texture*			GetSkyBoxTexture();
			virtual IScene*				GetScene();
			const char *				VGetName();
			virtual RenderDevice*		GetRenderDevice() override;
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
			IContext * m_pContext;
			std::unique_ptr<RenderPass> m_DefaultPass;
			std::list<std::unique_ptr<RenderPass>> m_ExtraPass;
			IScene* m_pScene;
		};
	}
}