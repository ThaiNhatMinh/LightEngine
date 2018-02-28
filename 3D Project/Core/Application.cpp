#include "pch.h"
#include "..\Graphics3D\OpenGLRenderer.h"
#include <type_traits>
void Application::SetupSubmodule()
{
	//E_DEBUG("Application StartUp...");
	m_Context = std::unique_ptr<Context>(new Context());
	Actor::m_Context = m_Context.get();
	ActorComponent::m_Context = m_Context.get();
	ISubSystem::m_Context = m_Context.get();

	m_pWindows = std::unique_ptr<Windows>(new Windows());
	m_pRenderer = std::unique_ptr<OpenGLRenderer>(new OpenGLRenderer());
	m_pEventManager = std::unique_ptr<EventManager>(new EventManager());
	m_pTimer = std::unique_ptr<GameTimer>(new GameTimer());
	m_pResources = std::unique_ptr<Resources>(new Resources());
	m_pInput = std::unique_ptr<DirectInput>(new DirectInput());
	m_pPhysic = std::unique_ptr<BulletPhysics>(new BulletPhysics());
	m_pConsole = std::unique_ptr<Console>(new Console());
	m_pDebuger = std::unique_ptr<Debug>(new Debug());
	m_pSystemUI = std::unique_ptr<SystemUI>(new SystemUI());
	m_pActorFactory = std::unique_ptr<ActorFactory>(new ActorFactory());
	m_pEffectSystem = std::unique_ptr<EffectSystem>(new EffectSystem());
	m_pSoundEngine = std::unique_ptr<SoundEngine>(new SoundEngine());
	

	// Init Windows
	m_pWindows->Init(m_Context.get());
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

	

	m_pConsole->RegisterVar("debug_physic", &m_DebugPhysic, 1, sizeof(int), TYPE_INT);

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

	Scene			*pScene = m_Game->GetScene();

	
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
			m_Game->Update(dt);
			// Update Physic
			m_pPhysic->VOnUpdate(dt);
			// Update Effect
			m_pEffectSystem->Update(pScene, dt);
			// Update Object
			m_pPhysic->VSyncVisibleScene();
			// Update sound
			m_pSoundEngine->Update();
		}

		
		if (m_DebugPhysic) m_pPhysic->VRenderDiagnostics();
		

		m_pRenderer->Clear();

		// Draw Game
		m_Game->Render();
		// Draw Effect
		m_pEffectSystem->Render(pScene);
		// Draw Console
		m_pConsole->Draw();
		// Daw Debug
		m_pDebuger->Render();
		// Draw SystemUI
		m_pSystemUI->Render();
		m_pRenderer->SwapBuffer();


	}
}