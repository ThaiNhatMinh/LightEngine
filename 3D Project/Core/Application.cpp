#include <pch.h>
#include "..\Graphics3D\OpenGLRenderer.h"

#include <type_traits>
void Application::SetupSubmodule()
{
	//E_DEBUG("Application StartUp...");
	m_Context = std::unique_ptr<Context>(new Context());
	Actor::m_Context = m_Context.get();
	ActorComponent::m_Context = m_Context.get();
	ISubSystem::m_Context = m_Context.get();

	m_pWindows = std::unique_ptr<Windows>(new Windows(m_Context.get()));
	m_pRenderer = std::unique_ptr<OpenGLRenderer>(new OpenGLRenderer(m_Context.get()));
	m_pEventManager = std::unique_ptr<EventManager>(new EventManager(m_Context.get()));
	m_pActorFactory = std::unique_ptr<ActorFactory>(new ActorFactory(m_Context.get()));
	m_pSoundEngine = std::unique_ptr<SoundEngine>(new SoundEngine(m_Context.get()));
	m_pResources = std::unique_ptr<Resources>(new Resources(m_Context.get()));
	m_pSystemUI = std::unique_ptr<SystemUI>(new SystemUI(m_Context.get()));
	m_pInput = std::unique_ptr<DirectInput>(new DirectInput(m_Context.get()));
	m_pConsole = std::unique_ptr<Console>(new Console(m_Context.get()));
	m_pDebuger = std::unique_ptr<Debug>(new Debug(m_Context.get()));
	m_pPhysic = std::unique_ptr<BulletPhysics>(new BulletPhysics(m_Context.get()));
	m_pTimer = std::unique_ptr<GameTimer>(new GameTimer(m_Context.get()));
	m_pEffectSystem = std::unique_ptr<EffectSystem>(new EffectSystem(m_Context.get()));
	m_pVGUI = std::unique_ptr<VGUI>(new VGUI(m_Context.get()));
	/*
	// Init Windows
	m_pWindows->Init();
	// Init Renderer
	m_pRenderer->Init(m_Context.get());
	// Init Event system
	m_pEventManager->Init(m_Context.get());
	// Init factory
	m_pActorFactory->Init(m_Context.get());
	// init sound engine
	m_pSoundEngine->Init(m_Context.get());
	// init resource
	m_pResources->Init(m_Context.get());
	// init system UI
	m_pSystemUI->Init(m_Context.get());
	// init console
	m_pConsole->Init(m_Context.get());
	// init input
	m_pInput->Init(m_Context.get());
	// init debug renderer
	m_pDebuger->Init(m_Context.get());
	// init bullets physic
	m_pPhysic->Init(m_Context.get());
	// init timer
	m_pTimer->Init(m_Context.get());
	//init effect system
	m_pEffectSystem->Init(m_Context.get());
	// init VGUI
	m_pVGUI->Init(m_Context.get());
	*/
	// Create game plugin
	m_GamePlugins = std::unique_ptr<GamePluginManager>(new GamePluginManager(m_Context.get()));

	m_GamePlugins->LoadPlugin("FPSGame.dll");
	m_pConsole->RegisterVar("debug_physic", &m_DebugPhysic, 1, sizeof(int), TYPE_INT);
	m_pConsole->RegisterVar("debug_hitbox", &m_Context->DrawSkeleton, 1, sizeof(int), TYPE_INT);

}

Application::~Application()
{
	
}



void Application::MainLoop()
{
	Setup();
	SetupSubmodule();
	Start();

	m_bRunMainLoop = true;
	// PROBLEM: How every thing update ?
	// 1. Timer
	// 2. Event Manager
	// 3. Input 
	// 4. Physic
	// 5. 

	Scene			*pScene = m_GamePlugins->GetScene();

	
	m_Context->GetSystem<Windows>()->ShowWindows();

	m_pTimer->Reset();

	while (m_bRunMainLoop)
	{
		glfwPollEvents();
		// Update input
		m_pInput->Update();
		if (m_pInput->KeyDown(DIK_ESCAPE)|| m_Context->GetSystem<Windows>()->ShouldClose())	m_bRunMainLoop = false;
		// Timer
		m_pTimer->Tick();
		m_pSystemUI->NewFrame();

		ImGui::Text("FPS: %d", m_pTimer->GetFPS());
		// check if in console then don't update game
		if (!m_pConsole->CheckStatus())
		{

			float dt = m_pTimer->GetDeltaTime();
			// Update Event
			m_pEventManager->VUpdate(20);
			// Update Game
			m_GamePlugins->UpdateGame(dt);
			// Update Physic
			m_pPhysic->VOnUpdate(dt);
			// Update Effect
			m_pEffectSystem->Update(pScene,dt);
			// Update Object
			m_pPhysic->VSyncVisibleScene();
			// Update sound
			m_pSoundEngine->Update();
			// Update Debuger
			m_pDebuger->Update();
			// Update VGUI
			m_pVGUI->Update(dt);
		}

		
		if (m_DebugPhysic) m_pPhysic->VRenderDiagnostics();
		

		m_pRenderer->Clear();

		// Draw Game
		m_GamePlugins->RenderGame();
		// Draw Effect
		m_pEffectSystem->Render(pScene);
		// Draw Console
		m_pConsole->Draw();
		// Daw Debug
		m_pDebuger->Render(pScene);
		// Draw SystemUI
		m_pSystemUI->Render();
		// Draw VGUI
		m_pVGUI->Render();

		m_pRenderer->SwapBuffer();


	}
}