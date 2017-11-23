#include "pch.h"
#include "..\Graphics3D\OpenGLRenderer.h"

void Application::SetupSubmodule()
{
	//E_DEBUG("Application StartUp...");
	// Event Manager must be startup first
	EventManager::startUp();

	GameTimer::startUp();
	OpenGLRenderer* p = new OpenGLRenderer;
	p->Init();

	Resources::startUp();

	DirectInput::startUp(p->GetWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	
	
	BulletPhysics::startUp();
	
	// test code
	

	Shader* pShader = gResources()->LoadShader<PrimShader>("NoTexture", "GameAssets\\SHADER\\NoTexture.vs", "GameAssets\\SHADER\\NoTexture.fs");
	gResources()->LoadShader<Shader>("Debug", "GameAssets\\SHADER\\Debug.vs", "GameAssets\\SHADER\\Debug.fs");
	Shader* pShader2 = gResources()->LoadShader<SkeShader>("SkeShader", "GameAssets\\SHADER\\Skeleton.vs", "GameAssets\\SHADER\\Texture.fs");
	gResources()->LoadShader<PrimShader>("Texture", "GameAssets\\SHADER\\Texture.vs", "GameAssets\\SHADER\\Texture.fs");
	
	
	m_pScene = new Scene(p);
	ActorFactory& factory = m_pScene->GetActorFactory();
	
	

	//Actor* p2 = ;
	
	//Actor* p4 = factory.CreateActor("GameAssets\\Camera.xml", nullptr, nullptr);
	//Actor* p5 = factory.CreateActor<TerrainWorld>("GameAssets\\Terrain.xml", nullptr, nullptr);

	m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\Player.xml", nullptr, nullptr));
	//pp = factory.CreateActor("GameAssets\\Zombie.xml", nullptr, nullptr);
	//m_pScene->GetRoot()->VAddChild(pp);
	//mat4 t = glm::translate(mat4(), vec3(0, 0, 100));
	//m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\Player.xml", nullptr, &t));
	//t = glm::translate(mat4(), vec3(0, 0, 200));
	//m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\Player.xml", nullptr, &t));

	//m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\PlayerWoman.xml", nullptr,nullptr));
	
	//m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\ZombieAssassin.xml", nullptr, nullptr));
	//m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\Ground.xml",nullptr,nullptr));
	m_pScene->GetRoot()->VAddChild(factory.CreateActor("GameAssets\\Box.xml", nullptr, nullptr));
	m_pScene->GetRoot()->VAddChild(factory.CreateActor<TerrainWorld>("GameAssets\\Terrain.xml", nullptr, nullptr));
	//anim = 0;
	//EvtData_SetAnimation* pE = new EvtData_SetAnimation(pp->GetId(), anim, true);
	//gEventManager()->VQueueEvent(pE);

}

Application::~Application()
{
	//E_DEBUG("Application ShutDown...");

	
	delete m_pScene;
	
	DirectInput::shutDown();
	GameTimer::shutDown();
	Resources::shutDown();

	
	
	BulletPhysics::shutDown();
	EventManager::shutDown();
	
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
	
	//glPolygonMode(GL_BACK, GL_LINE);
	bool m_bUpdatePhysic = false;
	gTimer()->Reset();

	while (m_bRunMainLoop)
	{

		if (gInput()->KeyDown(DIK_ESCAPE))
		{
			//glfwSetWindowShouldClose(m_pWindow->Window(), GLFW_TRUE);
			m_bRunMainLoop = false;
		}
		glfwPollEvents();

		
		gTimer()->Tick();

		gEventManager()->VUpdate(20);

		gInput()->Update();

		/*if (gInput()->KeyDown(DIK_P))
		{
			anim++;
			EvtData_SetAnimation* pE = new EvtData_SetAnimation(pp->GetId(), anim, true);
			gEventManager()->VQueueEvent(pE);
		}
		else if (gInput()->KeyDown(DIK_L))
		{
			anim--;
			if (anim < 0) anim = 0;
			EvtData_SetAnimation* pE = new EvtData_SetAnimation(pp->GetId(), anim, true);
			gEventManager()->VQueueEvent(pE);
		}*/

		gPhysic()->VOnUpdate(gTimer()->GetDeltaTime());
		gPhysic()->VSyncVisibleScene();
		//cout << gTimer()->GetFPS() << endl;
		
		
		
		//m_pGameLogic->Update();
		//m_pGameLogic->Render();

		m_pScene->OnUpdate(gTimer()->GetDeltaTime());
		m_pScene->OnRender();
		


	}
	//glfwTerminate();
}