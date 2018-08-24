#pragma once
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "ICamera.h"

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
		std::vector<render::ICamera*>		m_CameraList;
		render::ICamera*					m_CurrentCamera;


		IContext*			m_Context;

	public:
		Scene(IContext* c);
		~Scene();

		bool VLoadScene(const string& filename);
		bool VOnRender();

		bool VOnUpdate(float dt);
		bool VPostUpdate();

		render::ICamera*	VGetCurrentCamera();
		void		VSetCurrentCamera(render::ICamera * cam);
		IActor*		VGetRoot() { return m_pRoot.get(); };
		render::DirectionLight		GetDirLight() { return m_DirectionLight; };
	};
}