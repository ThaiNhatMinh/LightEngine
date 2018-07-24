#include <pch.h>
#include "Application.h"
#include <type_traits>

#include "OpenGLWindows.h"
#include "EventManager.h"
#include "ActorFactory.h"
#include "Timer.h"
#include "Context.h"
#include "..\Graphics3D\OpenGL\OpenGLRenderDevice.h"
#include "..\Graphics3D\OpenGL\OpenGLPipeline.h"
#include "..\Graphics3D\OpenGL\OpenGLVertexShader.h"
#include "..\Graphics3D\OpenGL\OpenGLPixelShader.h"
#include "..\ResourceManager\ResourceManager.h"
#include "..\Core\OpenGLInput.h"


void Application::SetupSubmodule()
{
	E_DEBUG("Application StartUp...");

	m_Context = std::unique_ptr<Light::IContext>(new Light::Context());
	

	m_pWindows = std::unique_ptr<Light::IWindow>(new Light::OpenGLWindows(m_Context.get()));
	m_pRenderer = std::unique_ptr<Light::render::RenderDevice>(new Light::render::OpenGLRenderDevice(m_Context.get()));
	m_pEventManager = std::unique_ptr<Light::IEventManager>(new Light::EventManager(m_Context.get()));
	m_pActorFactory = std::unique_ptr<Light::IFactory>(new Light::ActorFactory(m_Context.get()));
	//m_pSoundEngine = std::unique_ptr<SoundEngine>(new SoundEngine(m_Context.get()));
	m_pResources = std::unique_ptr<IResourceManager>(new Light::resources::ResourceManager(m_Context.get()));
	//m_pSystemUI = std::unique_ptr<SystemUI>(new SystemUI(m_Context.get()));
	m_pInput = std::unique_ptr<Light::IInput>(new Light::OpenGLInput(m_Context.get()));
	//m_pConsole = std::unique_ptr<Console>(new Console(m_Context.get()));
	//m_pDebuger = std::unique_ptr<Debug>(new Debug(m_Context.get()));
	//m_pPhysic = std::unique_ptr<BulletPhysics>(new BulletPhysics(m_Context.get()));
	m_pTimer = std::unique_ptr<Light::ITimer>(new Light::GameTimer(m_Context.get()));
	//m_pEffectSystem = std::unique_ptr<EffectSystem>(new EffectSystem(m_Context.get()));
	//m_pVGUI = std::unique_ptr<VGUI>(new VGUI(m_Context.get()));
	
	
	//m_pConsole->RegisterVar("debug_physic", &m_DebugPhysic, 1, sizeof(int), TYPE_INT);
	//m_pConsole->RegisterVar("debug_hitbox", &m_Context->DrawSkeleton, 1, sizeof(int), TYPE_INT);

}

Application::~Application()
{
	
}



void Application::MainLoop()
{
	
	SetupSubmodule();


						

	
	m_bRunMainLoop = true;
	// PROBLEM: How every thing update ?
	// 1. Timer
	// 2. Event Manager
	// 3. Input 
	// 4. Physic
	// 5. 

	//Scene			*pScene = m_GamePlugins->GetScene();

	
	//m_Context->GetSystem<Windows>()->ShowWindows();

	m_pTimer->VReset();

	IGamePlugin* pGame = m_GamePlugins.LoadPlugin();

	pGame->Init(m_Context.get());

	while (m_bRunMainLoop)
	{
		glfwPollEvents();
		// Update input
		//		m_pInput->VUpdate();
		if (m_pInput->VOnKey(Light::Escape))	m_bRunMainLoop = false;
		// Timer
		m_pTimer->VTick();
		//m_pSystemUI->NewFrame();

		//ImGui::Text("FPS: %d", m_pTimer->GetFPS());
		// check if in console then don't update game
		//if (!m_pConsole->CheckStatus())
		//{

		//	float dt = m_pTimer->GetDeltaTime();
		//	// Update Event
		m_pEventManager->VUpdate(20);
		//	// Update Game
		//	m_GamePlugins->UpdateGame(dt);
		//	// Update Physic
		//	m_pPhysic->VOnUpdate(dt);
		//	// Update Effect
		//	m_pEffectSystem->Update(pScene,dt);
		//	// Update Object
		//	m_pPhysic->VSyncVisibleScene();
		//	// Update sound
		//	m_pSoundEngine->Update();
		//	// Update Debuger
		//	m_pDebuger->Update();
		//	// Update VGUI
		//	m_pVGUI->Update(dt);
		//}

		//
		//if (m_DebugPhysic) m_pPhysic->VRenderDiagnostics();
		

		m_pRenderer->Clear();
		
		// draw our first triangle
		
		pGame->Render();
		// Draw Game
		//m_GamePlugins->RenderGame();
		// Draw Effect
		//m_pEffectSystem->Render(pScene);
		// Draw Console
		//m_pConsole->Draw();
		// Daw Debug
		//m_pDebuger->Render(pScene);
		// Draw SystemUI
		//m_pSystemUI->Render();
		// Draw VGUI
		//m_pVGUI->Render();

		//m_pRenderer->SwapBuffer();

		m_pWindows->VSwapBuffer();


	}

	pGame->ShutDown();
}