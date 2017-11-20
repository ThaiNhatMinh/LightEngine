#include "pch.h"
#include "..\Graphics3D\OpenGLRenderer.h"

void Application::SetupSubmodule()
{
	E_DEBUG("Application StartUp...");
	// Event Manager must be startup first
	EventManager::startUp();

	GameTimer::startUp();
	OpenGLRenderer* pRender = new OpenGLRenderer;
	pRender->Init();

	Resources::startUp();

	DirectInput::startUp(pRender->GetWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	//LuaStateManager::startUp();
	//ScriptExports::Register();
	
	BulletPhysics::startUp();
	
	// test code
	

	Shader* pShader = gResources()->LoadShader<PrimShader>("NoTexture", "GameAssets\\SHADER\\NoTexture.vs", "GameAssets\\SHADER\\NoTexture.fs");
	gResources()->LoadShader<Shader>("Debug", "GameAssets\\SHADER\\Debug.vs", "GameAssets\\SHADER\\Debug.fs");
	Shader* pShader2 = gResources()->LoadShader<SkeShader>("SkeShader", "GameAssets\\SHADER\\Skeleton.vs", "GameAssets\\SHADER\\Texture.fs");
	gResources()->LoadShader<PrimShader>("Texture", "GameAssets\\SHADER\\Texture.vs", "GameAssets\\SHADER\\Texture.fs");
	
	
	m_pScene = new Scene(pRender);
	ActorFactory& factory = m_pScene->GetActorFactory();
	
	
	
	

	/*
	No longer need this. Delete it if you want
	Camera* cam = new Camera(vec3(0,0,20),vec3(0,0,0),vec3(0,1,0));
	Frustum* frustum = new Frustum(52.0f, 4.0f / 3.0f, 1.0f, 1000.0f);
	m_pScene->SetCamera(cam);
	m_pScene->SetFrustum(frustum);
	*/
	

	Actor* p1 = factory.CreateActor("GameAssets\\Player.xml", nullptr,nullptr);
	//Actor* p2 = factory.CreateActor("GameAssets\\Ground.xml",nullptr,nullptr);
	//Actor* p3 = factory.CreateActor("GameAssets\\Box.xml", nullptr, nullptr);
	//Actor* p4 = factory.CreateActor("GameAssets\\Camera.xml", nullptr, nullptr);
	Actor* p5 = factory.CreateActor<TerrainWorld>("GameAssets\\Terrain.xml", nullptr, nullptr);

	m_pScene->GetRoot()->VAddChild(p1);
	//m_pScene->GetRoot()->VAddChild(p2);
	//m_pScene->GetRoot()->VAddChild(p3);
	//m_pScene->GetRoot()->VAddChild(p4);
	m_pScene->GetRoot()->VAddChild(p5);


}

Application::~Application()
{
	E_DEBUG("Application ShutDown...");

	
	delete m_pScene;
	
	DirectInput::shutDown();
	GameTimer::shutDown();
	Resources::shutDown();

	//ScriptExports::Unregister();
	//LuaStateManager::shutDown();
	BulletPhysics::shutDown();
	EventManager::shutDown();
	
}


const Debug & Application::GetDebug()
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
	
	glPolygonMode(GL_BACK, GL_LINE);
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

		gPhysic()->VOnUpdate(gTimer()->GetDeltaTime());
		gPhysic()->VSyncVisibleScene();
		//cout << gTimer()->GetFPS() << endl;
		
		//gPhysic()->VRenderDiagnostics();
		
		//m_pGameLogic->Update();
		//m_pGameLogic->Render();

		m_pScene->OnUpdate(gTimer()->GetDeltaTime());
		m_pScene->OnRender();
		


	}

}