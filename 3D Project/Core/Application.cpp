#include "pch.h"
#include "..\Graphics3D\OpenGLRenderer.h"
#include <type_traits>

/*void Application::SetupSubmodule()
{
	//E_DEBUG("Application StartUp...");
	Context			*C = new Context();
	Windows			*W = new Windows();
	OpenGLRenderer	*O = new OpenGLRenderer();
	EventManager	*E = new EventManager();
	GameTimer		*G = new GameTimer();
	Resources		*R = new Resources();
	DirectInput		*D = new DirectInput();
	BulletPhysics	*B = new BulletPhysics();
	Console			*Con = new Console();
	Debug			*Db = new Debug();
	SystemUI		*S = new SystemUI();
	ActorFactory	*A = new ActorFactory();
	EffectSystem	*ES = new EffectSystem();
	SoundEngine		*SE = new SoundEngine();
	Actor::m_Context = C;
	ActorComponent::m_Context = C;

	// Init Windows
	W->Init(C);
	// Init Renderer
	O->Init(C);
	// Init Event system
	E->Init(C);
	// Init factory
	A->Init(C);
	// init sound engine
	SE->Init(C);
	// init resource
	R->Init(C);
	// init system UI
	S->Init(C);
	// init console
	Con->Init(C);
	// init input
	D->Init(C);
	// init debug renderer
	Db->Init(C);
	// init bullets physic
	B->Init(C);
	// init timer
	G->Init(C);
	//init effect system
	ES->Init(C);

	m_Context = std::unique_ptr<Context>(C);

	Con->RegisterVar("debug_physic", &m_DebugPhysic, 1, sizeof(int), TYPE_INT);

	m_VGUI = std::unique_ptr<VGUI>(new VGUI(m_Context.get()));
	m_GamePlugins = std::unique_ptr<GamePluginManager>(new GamePluginManager(m_Context.get()));
}*/

Application::Application() :m_Context(), m_Windows(new Windows(&m_Context)), m_Input(new DirectInput(&m_Context)),
m_Renderer(new  OpenGLRenderer(&m_Context)), m_EventManager(new  EventManager(&m_Context)), m_Factory(new ActorFactory(&m_Context)),
m_SoundEngine(new  SoundEngine(&m_Context)), m_Resources(new  Resources(&m_Context)), m_SystemUI(new SystemUI (&m_Context)),
m_Console(new  Console(&m_Context)), m_DebugRender(new  Debug(&m_Context)), m_Physic(new  BulletPhysics(&m_Context)),
m_Timer(new  GameTimer(&m_Context)), m_EffectSystem(new EffectSystem (&m_Context)), m_VGUI(new  VGUI(&m_Context)),m_GamePlugins(&m_Context)
{
	Actor::m_Context = &m_Context;
	ActorComponent::m_Context = &m_Context;
	m_Console->RegisterVar("debug_physic", &m_DebugPhysic, 1, sizeof(int), TYPE_INT);
}

Application::~Application()
{
	
}



void Application::MainLoop()
{
	//SetupSubmodule();
	Start();

	m_bRunMainLoop = true;
	// PROBLEM: How every thing update ?
	// 1. Timer
	// 2. Event Manager
	// 3. Input 
	// 4. Physic
	// 5. 
	
	/*EventManager	*E = m_Context->m_pEventManager.get();
	GameTimer		*G = m_Context->m_pTimer.get();
	DirectInput		*D = m_Context->m_pInput.get();
	BulletPhysics	*B = m_Context->m_pPhysic.get();
	OpenGLRenderer	*O = m_Context->m_pRenderer.get();
	Console			*C = m_Context->m_pConsole.get();
	SystemUI		*S = m_Context->m_pSystemUI.get();
	Debug			*Db = m_Context->m_pDebuger.get();
	EffectSystem	*ES = m_Context->m_pEffectSystem.get();
	SoundEngine		*SE = m_Context->m_pSoundEngine.get();*/
	Scene			*pScene = m_Game->GetScene();

	
	/*auto b = Sprite(m_Context->m_pResources->GetTexture("TEXTURES\\ESCAPE.DTX"));
	b.GetPos() = vec3(100, 150, 100);
	ES->AddSprite(b);

	auto d = Sprite(m_Context->m_pResources->GetTexture("TEXTURES\\SGFX_se_decal_04.DTX"));
	d.GetPos() = vec3(100, 150, 300);
	ES->AddSprite(d);

	auto a = m_Context->m_pResources->GetSpriteAnimation("BLOOD3.SPR");
	a->GetPos() = vec3(200, 150, 0);
	ES->AddSprite(a);

	auto c = m_Context->m_pResources->GetSpriteAnimation("EX.SPR");
	c->GetPos() = vec3(250, 350, 50);
	ES->AddSprite(c);

	
	c = m_Context->m_pResources->GetSpriteAnimation("SGFX_se_fire_explode_01.SPR");
	c->GetPos() = vec3(350, 250, 100);
	ES->AddSprite(c);*/
	
	m_Windows->ShowWindows();

	m_Timer->Reset();

	while (m_bRunMainLoop)
	{
		glfwPollEvents();
		// Update input
		m_Input->Update();
		if (m_Input->KeyDown(DIK_ESCAPE)|| m_Windows->ShouldClose())	m_bRunMainLoop = false;
		// Timer
		m_Timer->Tick();
		m_SystemUI->NewFrame();
		//ImGui::Text("FPS: %d",m_Timer->GetFPS());
		cout << m_Timer->GetFPS() << endl;
		float dt = m_Timer->GetDeltaTime();
		// check if in console then don't update game
		if (!m_Console->CheckStatus())
		{


			// Update Event
			m_EventManager->VUpdate(20);
			// Update Game
			m_Game->Update(dt);
			// Update Physic
			m_Physic->VOnUpdate(dt);
			// Update Effect
			m_EffectSystem->Update(pScene, dt);
			// Update Object
			m_Physic->VSyncVisibleScene();
			// Update sound
			m_SoundEngine->Update();
		}

		
		if (m_DebugPhysic) m_Physic->VRenderDiagnostics();
		

		m_Renderer->Clear();

		// Draw Game
		m_Game->Render();
		// Draw Effect
		m_EffectSystem->Render(pScene);
		// Draw Console
		m_Console->Draw();
		// Daw Debug
		m_DebugRender->Render();
		// Draw SystemUI
		m_SystemUI->Render();
		// Draw UI
		m_VGUI->Render();

		m_Renderer->SwapBuffer();


	}
}