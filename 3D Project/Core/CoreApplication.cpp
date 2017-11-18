#include "pch.h"
#include "..\Graphics3D\OpenGLRenderer.h"

void CoreApplication::onStartUp()
{
	E_DEBUG("Application StartUp...");

	ActorFactory::startUp();

	GameTimer::startUp();
	Resources::startUp();
	OpenGLRenderer* pRender = new OpenGLRenderer;
	pRender->Init();
	// Event Manager must be startup first
	EventManager::startUp();
	DirectInput::startUp(pRender->GetWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	//LuaStateManager::startUp();
	//ScriptExports::Register();
	
	BulletPhysics::startUp();
	
	// test code
	

	Shader* pShader = gResources()->LoadShader<PrimShader>("NoTexture", "GameAssets\\SHADER\\NoTexture.vs", "GameAssets\\SHADER\\NoTexture.fs");
	gResources()->LoadShader<Shader>("Debug", "GameAssets\\SHADER\\Debug.vs", "GameAssets\\SHADER\\Debug.fs");
	Shader* pShader2 = gResources()->LoadShader<SkeShader>("SkeShader", "GameAssets\\SHADER\\Skeleton.vs", "GameAssets\\SHADER\\Texture.fs");
	
	
	m_pScene = new Scene(pRender);

	
	
	
	Debug::startUp(m_pScene);

	Camera* cam = new Camera(vec3(0,0,20),vec3(0,0,0),vec3(0,1,0));
	Frustum* frustum = new Frustum(52.0f, 4.0f / 3.0f, 1.0f, 1000.0f);
	m_pScene->SetCamera(cam);
	m_pScene->SetFrustum(frustum);

	

	Actor* p1 = gActorFactory()->CreateActor("GameAssets\\player_teapot.xml", nullptr,nullptr);
	Actor* p2 = gActorFactory()->CreateActor("GameAssets\\Ground.xml",nullptr,nullptr);
	Actor* p3 = gActorFactory()->CreateActor("GameAssets\\Box.xml", nullptr, nullptr);
	//cam->SetCameraActor(p3);
	m_pScene->GetRoot()->VAddChild(p1);
	m_pScene->GetRoot()->VAddChild(p2);
	m_pScene->GetRoot()->VAddChild(p3);
	//gResources()->LoadModelXML("GameAssets\\MODEL\\707.xml");
	
	//IEvent* pEvent = new EvtData_SetAnimation(p1->GetId(), sniper +idle,1);
	//gEventManager()->VQueueEvent(pEvent);

}

void CoreApplication::onShutDown()
{
	E_DEBUG("Application ShutDown...");

	
	delete m_pScene;
	
	DirectInput::shutDown();
	ActorFactory::shutDown();
	GameTimer::shutDown();
	Resources::shutDown();

	//ScriptExports::Unregister();
	//LuaStateManager::shutDown();
	BulletPhysics::shutDown();
	EventManager::shutDown();
	Debug::shutDown();
	
	//delete m_pWindow;
}

void CoreApplication::SetGameLogic(BaseGameLogic * pGameLogic)
{
	m_pGameLogic = pGameLogic;
}

bool CoreApplication::MainLoop()
{

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

	return false;
}
