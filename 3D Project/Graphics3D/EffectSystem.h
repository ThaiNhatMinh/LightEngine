#pragma once
#include "..\Interface\IEffectSystem.h"
#include "..\Interface\IResourceManager.h"
#include "..\Interface\IRenderSystem.h"
#include "..\Interface\IEvent.h"
#include "Sprite.h"
namespace Light
{
	class EffectSystem : public IEffectSystem
	{
	private:
		render::VertexArray*			VAO;
		render::VertexBuffer*		VBO;		
		std::vector<std::unique_ptr<render::Sprite>> m_SpriteLists;
		std::list<render::BaseParticle*> m_Particles;
		render::Pipeline* m_pShader;
		render::PipelineParam* m_uMVP,*m_uCamUp,*m_uCamRight;
		render::RenderDevice* m_pRenderer;
		render::IRenderSystem* m_pRS;
		resources::IResourceManager* m_pResources;

	private:
		void CreateSpriteEvent(std::shared_ptr<IEvent> pEvent);

		void UpdateSprite(float dt, render::ICamera*);
		void RenderSprite();
	public:
		virtual const char* VGetName();
		EffectSystem(IContext* c);
		~EffectSystem();

		
		void			VUpdate(float dt);
		void			VRender();

	
	};
}