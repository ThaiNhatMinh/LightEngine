#pragma once
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "ICamera.h"
#include "SkyBox.h"

#include "..\Interface\IActor.h"
#include "..\Interface\IScene.h"

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
		render::DirectionLight					m_DirectionLight; // only one direction light


		IContext*			m_Context;
		std::string			m_Name;
		render::SkyBox		m_SkyBox;
	public:
		Scene(IContext* c,const std::string& name);
		~Scene();

		virtual bool VLoad(const std::string& file)override;
		bool VOnRender();

		bool VOnUpdate(float dt);
		bool VPostUpdate();
		IActor*		VGetRoot() { return m_pRoot.get(); };
		render::DirectionLight		GetDirLight() { return m_DirectionLight; };
		virtual std::string VGetSceneName()override;
		render::SkyBox* GetSkyBox();
	};
}