#pragma once
#include "..\GamePlugin\GamePluginManager.h"

#include "..\Interface\IWindow.h"
#include "..\Interface\IFactory.h"
#include "..\Interface\ISoundEngine.h"
#include "..\Interface\IResourceManager.h"
//#include "..\Interface\ISysUI.h"
//#include "..\Interface\IConsole.h"
#include "..\Interface\IGamePhysic.h"

#include "RenderSystem.h"
#include "Timer.h"
#include "EventManager.h"
#include "Physic.h"
#include "OpenGLInput.h"
#include "Context.h"
#include "..\Script\LuaScriptExporter.h"
namespace Light
{
	class Application
	{
	public:
		Application() {};
		~Application();




		void MainLoop();
	private:
		
		void SetupSubmodule();
	protected:
		std::unique_ptr<Light::Context>					m_Context;
		std::unique_ptr<IWindow>						m_pWindows;
		std::unique_ptr<resources::IResourceManager>	m_pResources;
		std::unique_ptr<render::RenderSystem>			m_pRenderer;
		std::unique_ptr<EventManager>					m_pEventManager;
		std::unique_ptr<physics::BulletPhysics>			m_pPhysic;
		std::unique_ptr<IFactory>						m_pActorFactory;
		std::unique_ptr<ISoundEngine>					m_pSoundEngine;
		
		//std::unique_ptr<ISysUI>							m_pSystemUI;
		//std::unique_ptr<IConsole>						m_pConsole;
		std::unique_ptr<OpenGLInput>							m_pInput;

		
		std::unique_ptr<GameTimer>							m_pTimer;
		//std::unique_ptr<VGUI>							m_pVGUI;

		std::unique_ptr<LuaScriptManager>				m_pScriptManager;
		GamePluginManager								m_GamePlugins;
	};

}