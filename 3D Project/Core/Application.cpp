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
	

	W->Init(C);
	O->Init(C);
	R->Init(C);
	Con->Init(C);
	S->Init(C);
	E->Init(C);
	G->Init(C);
	
	D->Init(C);
	B->Init(C);
	Db->Init(C);
	A->Init(C);

	Actor::m_Context = C;
	ActorComponent::m_Context = C;
	ISubSystem::m_Context = C;
	m_Context = std::unique_ptr<Context>(C);

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

		

		
		

		B->VOnUpdate(G->GetDeltaTime());
		B->VSyncVisibleScene();
		//cout << gTimer()->GetFPS() << endl;
		//glPolygonMode(GL_FRONT, GL_LINE);
		
		m_Game->Update(G->GetDeltaTime());

		O->Clear();

		m_Game->Render();

		C->Draw();
		
		
		Db->Render();
		S->Render();
		O->SwapBuffer();


	}
}