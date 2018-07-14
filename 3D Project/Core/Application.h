#pragma once
#include "..\GamePlugin\GamePluginManager.h"

#include "..\Interface\IContext.h"
#include "..\Interface\IWindow.h"
//#include "..\Interface\IEventManager.h"
//#include "..\Interface\ISoundEngine.h"
#include "..\Interface\IResourceManager.h"
//#include "..\Interface\ISysUI.h"
//#include "..\Interface\IConsole.h"
#include "..\Interface\IInput.h"
//#include "..\Interface\IDebugRender.h"
//#include "..\Interface\IGamePhysic.h"
//#include "..\Interface\ITimer.h"
#include "..\Graphics3D\Renderer.h"

using Light::render::RenderDevice;
using Light::IWindow;
//using Light::IEventManager;
//using Light::IFactory;
//using Light::ISoundEngine;
using Light::resources::IResourceManager;
//using Light::ISysUI;
//using Light::IConsole;
using Light::IInput;
//using Light::IDebugRender;
//using Light::physics::IGamePhysic;
//using Light::ITimer;

class Application 
{
public:
	Application(){};
	~Application();
	

	
	
	void MainLoop();
private:
	bool m_bRunMainLoop;
	void SetupSubmodule();
	
protected:
	std::unique_ptr<Light::IContext>			m_Context;
	std::unique_ptr<IWindow>			m_pWindows;
	std::unique_ptr<RenderDevice>		m_pRenderer;
	//std::unique_ptr<IEventManager>		m_pEventManager;
	//std::unique_ptr<IFactory>			m_pActorFactory;
	//std::unique_ptr<ISoundEngine>		m_pSoundEngine;
	std::unique_ptr<IResourceManager>	m_pResources;
	//std::unique_ptr<ISysUI>				m_pSystemUI;
	//std::unique_ptr<IConsole>			m_pConsole;
	std::unique_ptr<IInput>				m_pInput;

	//std::unique_ptr<IDebugRender>		m_pDebuger;
	//std::unique_ptr<IGamePhysic>		m_pPhysic;
	//std::unique_ptr<ITimer>				m_pTimer;
	//std::unique_ptr<EffectSystem>		m_pEffectSystem;
	//std::unique_ptr<VGUI>				m_pVGUI;

	
	GamePluginManager					m_GamePlugins;
};
