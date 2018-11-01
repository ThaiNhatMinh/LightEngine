#pragma once
#include "..\GamePlugin\GamePluginManager.h"

#include "..\Interface\IContext.h"
#include "..\Interface\IWindow.h"
#include "..\Interface\IEventManager.h"
#include "..\Interface\IFactory.h"
#include "..\Interface\ISoundEngine.h"
#include "..\Interface\IResourceManager.h"
//#include "..\Interface\ISysUI.h"
//#include "..\Interface\IConsole.h"
#include "..\Interface\IInput.h"
#include "..\Interface\IGamePhysic.h"
#include "..\Interface\ITimer.h"
#include "..\Interface\IRenderSystem.h"
#include "..\Interface\IScriptManager.h"

namespace Light
{
	class Application
	{
	public:
		Application() {};
		~Application();




		void MainLoop();
	private:
		bool m_bRunMainLoop;
		void SetupSubmodule();
	protected:
		std::unique_ptr<Light::IContext>				m_Context;
		std::unique_ptr<IWindow>						m_pWindows;
		std::unique_ptr<resources::IResourceManager>	m_pResources;
		std::unique_ptr<render::IRenderSystem>			m_pRenderer;
		std::unique_ptr<IEventManager>					m_pEventManager;
		std::unique_ptr<physics::IGamePhysic>			m_pPhysic;
		std::unique_ptr<IFactory>						m_pActorFactory;
		std::unique_ptr<ISoundEngine>					m_pSoundEngine;
		
		//std::unique_ptr<ISysUI>							m_pSystemUI;
		//std::unique_ptr<IConsole>						m_pConsole;
		std::unique_ptr<IInput>							m_pInput;

		
		std::unique_ptr<ITimer>							m_pTimer;
		//std::unique_ptr<VGUI>							m_pVGUI;

		std::unique_ptr<IScriptManager>					m_pScriptManager;
		GamePluginManager								m_GamePlugins;
	};

}