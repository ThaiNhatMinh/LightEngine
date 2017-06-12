#include "..\pch.h"


void CoreApplication::onStartUp()
{
	E_DEBUG("Application StartUp...");
	m_pWindow = new Windows("Ex-T1", 800, 600);
	m_pWindow->InitWindow();
	// Event Manager must be startup first
	
	EventManager::startUp();

	DirectInput::startUp(m_pWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	ActorFactory::startUp();
	GameTimer::startUp();
	Resources::startUp();

	BulletPhysics::startUp();


	// test code
	m_pScene = new Scene();
	
	Debug::startUp(m_pScene);

	Camera* cam = new Camera(vec3(0,0,20),vec3(0,0,0),vec3(0,1,0));
	Frustum* frustum = new Frustum(52.0f, 4.0f / 3.0f, 1.0f, 1000.0f);
	m_pScene->SetCamera(cam);
	m_pScene->SetFrustum(frustum);
	
	Actor* p1 = gActorFactory()->CreateActor("GameAssets\\player_teapot.xml", nullptr,nullptr);
	//p2 = gActorFactory()->CreateActor("GameAssets\\Ground.xml",nullptr,nullptr);
	//p3 = gActorFactory()->CreateActor("GameAssets\\Box.xml", nullptr, nullptr);
	m_pScene->GetRoot()->VAddChild(p1);
	//m_pScene->GetRoot()->VAddChild(p2);
	//m_pScene->GetRoot()->VAddChild(p3);
	//gResources()->LoadModelXML("GameAssets\\MODEL\\707.xml");
	//Shader* pShader = gResources()->LoadShader<PrimShader>("NoTexture", "GameAssets\\SHADER\\NoTexture.vs", "GameAssets\\SHADER\\NoTexture.fs");
	gResources()->LoadShader<Shader>("Debug", "GameAssets\\SHADER\\Debug.vs", "GameAssets\\SHADER\\Debug.fs");
	Shader* pShader2 = gResources()->LoadShader<SkeShader>("SkeShader", "GameAssets\\SHADER\\Skeleton.vs", "GameAssets\\SHADER\\Texture.fs");
	p1->VSetShader(pShader2);
	p1->PostInit();
	//p2->VSetShader(pShader);
	//p2->PostInit();
	//p3->VSetShader(pShader);
	//p3->PostInit();
	
}

void CoreApplication::onShutDown()
{
	E_DEBUG("Application ShutDown...");

	
	delete m_pScene;
	
	DirectInput::shutDown();
	ActorFactory::shutDown();
	GameTimer::shutDown();
	Resources::shutDown();

	BulletPhysics::shutDown();
	EventManager::shutDown();
	Debug::shutDown();
	
	delete m_pWindow;
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
	bool m_bUpdatePhysic = false;
	gTimer()->Reset();
	while (m_bRunMainLoop)
	{
		// Check Exit
		if (glfwWindowShouldClose(m_pWindow->Window())) m_bRunMainLoop = false;
		if (gInput()->KeyDown(DIK_ESCAPE)) glfwSetWindowShouldClose(m_pWindow->Window(), GLFW_TRUE);
		if (gInput()->KeyDown(DIK_0))
		{
			m_bUpdatePhysic = !m_bUpdatePhysic;
		}
		glfwPollEvents();

		if (gInput()->KeyDown(DIK_SPACE))
		{
			
			gPhysic()->VApplyForce(vec3(0.0f, 1.0f, 0.0f), 10, p3->GetId());
		}
		gTimer()->Tick();

		gEventManager()->VUpdate(20);

		gInput()->Update();

		if(m_bUpdatePhysic) 	gPhysic()->VOnUpdate(gTimer()->GetDeltaTime());
		cout << gTimer()->GetFPS() << endl;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1, 0.1, 0.1, 1.0f);

		//gPhysic()->VRenderDiagnostics();
		//gDebug()->DrawLine(vec3(0.0f), vec3(100.0f), vec3(0.0f,1.0f,0.5f));

		m_pScene->OnUpdate(gTimer()->GetDeltaTime());

		m_pScene->OnRender();
		


		glfwSwapBuffers(m_pWindow->Window());
	}

	return false;
}
