#pragma once
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "..\Interface\ICamera.h"
#include "SkyBox.h"

#include "..\Interface\IActor.h"
#include "..\Interface\IScene.h"
#include "LightManager.h"


namespace Light
{
	static const char* SYSTEM_ROOT_ACTOR = "GameAssets\\System\\Root.xml";
	class Scene: public IScene
	{
	private:
		// Simple Object with only transform component
		// This store every thing in scene
		std::unique_ptr<IActor>	m_pRoot;
		//std::unique_ptr<Actor>	m_pSkyBox;
		
		render::LightManager m_LightManager;

		IContext*			m_Context;
		std::string			m_Name;
		render::SkyBox		m_SkyBox;
	public:
		Scene(IContext* c,const std::string& name);
		~Scene();

		bool					VLoad(const std::string& file)override;
		bool					VOnRender()override;

		bool					VOnUpdate(float dt)override;
		bool					VPostUpdate()override;
		std::string				VGetSceneName()override;

		IActor*					VGetRoot() { return m_pRoot.get(); };
		render::SkyBox*			GetSkyBox();
		render::LightManager*	GetLightManager();
	};
}