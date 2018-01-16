#include "pch.h"
#include "..\Graphics3D\OpenGLRenderer.h"
#include <type_traits>
void Application::SetupSubmodule()
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
	Actor::m_Context = C;
	ActorComponent::m_Context = C;
	ISubSystem::m_Context = C;

	
	W->Init(C);
	O->Init(C);
	
	
	E->Init(C);
	A->Init(C);
	R->Init(C);
	S->Init(C);
	Con->Init(C);

	
	D->Init(C);
	Db->Init(C);
	B->Init(C);
	

	G->Init(C);

	ES->Init(C);
	m_Context = std::unique_ptr<Context>(C);

	Con->RegisterVar("debug_physic", &m_DebugPhysic, 1, sizeof(int), TYPE_INT);

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
	
	EventManager	*E = m_Context->m_pEventManager.get();
	GameTimer		*G = m_Context->m_pTimer.get();
	DirectInput		*D = m_Context->m_pInput.get();
	BulletPhysics	*B = m_Context->m_pPhysic.get();
	OpenGLRenderer	*O = m_Context->m_pRenderer.get();
	Console			*C = m_Context->m_pConsole.get();
	SystemUI		*S = m_Context->m_pSystemUI.get();
	Debug			*Db = m_Context->m_pDebuger.get();
	EffectSystem	*ES = m_Context->m_pEffectSystem.get();
	Scene			*pScene = m_Game->GetScene();


	ES->AddSprite(Sprite(m_Context->m_pResources->GetTexture("TEXTURES\\ESCAPE.DTX")));

	m_Context->m_pWindows->ShowWindows();

	G->Reset();

	while (m_bRunMainLoop)
	{
		glfwPollEvents();
		D->Update();
		if (D->KeyDown(DIK_ESCAPE)|| m_Context->m_pWindows->ShouldClose())	m_bRunMainLoop = false;
		
		C->CheckStatus();

		

		
		G->Tick();

		S->NewFrame();

		E->VUpdate(20);

		

		m_Game->Update(G->GetDeltaTime());
		

		B->VOnUpdate(G->GetDeltaTime());
		B->VSyncVisibleScene();
		
		if (m_DebugPhysic) B->VRenderDiagnostics();
		

		O->Clear();

		// Draw Game
		m_Game->Render();
		// Draw Effect
		ES->Render(pScene);
		// Draw Console
		C->Draw();
		// Daw Debug
		Db->Render();
		// Draw SystemUI
		S->Render();
		O->SwapBuffer();


	}
}