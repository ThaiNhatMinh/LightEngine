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

	W->Init(C);
	O->Init(C);
	E->Init(C);
	G->Init(C);
	R->Init(C);
	D->Init(C);
	B->Init(C);
	Con->Init(C);

	Actor::m_Context = C;
	ActorComponent::m_Context = C;
	ISubSystem::m_Context = C;
	m_Context = std::unique_ptr<Context>(C);

}

Application::~Application()
{
	//E_DEBUG("Application ShutDown...");

	delete m_pScene;
	
}

void Application::Start()
{
	m_pScene = new Scene(m_Context.get());
	ActorFactory& factory = m_pScene->GetActorFactory();


	// test code


	//Shader* pShader = m_Context->m_pResources->LoadShader<PrimShader>("NoTexture", "GameAssets\\SHADER\\NoTexture.vs", "GameAssets\\SHADER\\NoTexture.fs");
	//m_Context->m_pResources->LoadShader<Shader>("Debug", "GameAssets\\SHADER\\Debug.vs", "GameAssets\\SHADER\\Debug.fs");
	//Shader* pShader2 = m_Context->m_pResources->LoadShader<SkeShader>("SkeShader", "GameAssets\\SHADER\\Skeleton.vs", "GameAssets\\SHADER\\Texture.fs");
	//m_Context->m_pResources->LoadShader<PrimShader>("Texture", "GameAssets\\SHADER\\Texture.vs", "GameAssets\\SHADER\\Texture.fs");



	//Actor* p2 = ;

	Actor* p4 = factory.CreateActor("GameAssets\\ACTOR\\Player.xml", nullptr, nullptr);
	m_pScene->GetRoot()->VAddChild(std::unique_ptr<Actor>(p4));
	//Actor* p5 = factory.CreateActor<TerrainWorld>("GameAssets\\Terrain.xml", nullptr, nullptr);
	Actor* pp;// = factory.CreateActor("GameAssets\\ACTOR\\Camera.xml", nullptr, nullptr);
			  //m_pScene->GetRoot()->VAddChild(std::unique_ptr<Actor>(pp));
			  //pp = factory.CreateActor("GameAssets\\Zombie.xml", nullptr, nullptr);
			  //m_pScene->GetRoot()->VAddChild(pp);
			  //mat4 t = glm::translate(mat4(), vec3(0, 0, 100));
			  //m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\Player.xml", nullptr, &t));
			  //t = glm::translate(mat4(), vec3(0, 0, 200));
			  //m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\Player.xml", nullptr, &t));

			  //m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\PlayerWoman.xml", nullptr,nullptr));

			  //m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\ZombieAssassin.xml", nullptr, nullptr));
			  //m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\Ground.xml",nullptr,nullptr));
			  //m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\Box.xml", nullptr, nullptr));
	pp = factory.CreateActor("GameAssets\\ACTOR\\Terrain.xml", nullptr, nullptr);
	m_pScene->GetRoot()->VAddChild(std::unique_ptr<Actor>(pp));
	pp = factory.CreateActor("GameAssets\\ACTOR\\PV.xml", nullptr, nullptr);
	m_pScene->GetRoot()->VAddChild(std::unique_ptr<Actor>(pp));
}


Debug & Application::GetDebug()
{
	// TODO: insert return statement here
	return m_pScene->GetDebug();
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
	m_Context->m_pWindows->ShowWindows();

	G->Reset();

	while (m_bRunMainLoop)
	{
		glfwPollEvents();
		D->Update();
		if (D->KeyDown(DIK_ESCAPE)|| m_Context->m_pWindows->ShouldClose())	m_bRunMainLoop = false;
		
		C->CheckStatus(D->KeyDown(DIK_GRAVE));

		

		
		G->Tick();

		E->VUpdate(20);

		

		

		B->VOnUpdate(G->GetDeltaTime());
		B->VSyncVisibleScene();
		//cout << gTimer()->GetFPS() << endl;
		
		
		m_pScene->OnUpdate(G->GetDeltaTime());

		O->Clear();

		m_pScene->OnRender();

		m_Context->m_pConsole->Draw();

		O->SwapBuffer();


	}
	//glfwTerminate();
}