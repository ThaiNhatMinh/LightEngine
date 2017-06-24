#include "..\pch.h"


void CoreApplication::onStartUp()
{
	E_DEBUG("Application StartUp...");
	m_pWindow = new Windows("Ex-T1", 800, 600);
	m_pWindow->InitWindow();
	// Event Manager must be startup first
	
	EventManager::startUp();

	DirectInput::startUp(m_pWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	GameTimer::startUp();
	Resources::startUp();

	LuaStateManager::startUp();
	ScriptExports::Register();
	ActorFactory::startUp();
	BulletPhysics::startUp();


	Shader* pShader = gResources()->LoadShader<PrimShader>("NoTexture", "GameAssets\\SHADER\\NoTexture.vs", "GameAssets\\SHADER\\NoTexture.fs");
	gResources()->LoadShader<Shader>("Debug", "GameAssets\\SHADER\\Debug.vs", "GameAssets\\SHADER\\Debug.fs");
	Shader* pShader2 = gResources()->LoadShader<SkeShader>("SkeShader", "GameAssets\\SHADER\\Skeleton.vs", "GameAssets\\SHADER\\Texture.fs");

	// test code
	m_pScene = new Scene();
	
	Debug::startUp(m_pScene);

	Camera* cam = new Camera(vec3(0,0,20),vec3(0,0,0),vec3(0,1,0));
	Frustum* frustum = new Frustum(52.0f, 4.0f / 3.0f, 1.0f, 1000.0f);
	m_pScene->SetCamera(cam);
	m_pScene->SetFrustum(frustum);

	

	Actor* p1 = gActorFactory()->CreateActor("GameAssets\\player_teapot.xml", nullptr,nullptr);
	p2 = gActorFactory()->CreateActor("GameAssets\\Ground.xml",nullptr,nullptr);
	p3 = gActorFactory()->CreateActor("GameAssets\\Box.xml", nullptr, nullptr);
	cam->SetCameraActor(p3);
	m_pScene->GetRoot()->VAddChild(p1);
	m_pScene->GetRoot()->VAddChild(p2);
	m_pScene->GetRoot()->VAddChild(p3);
	//gResources()->LoadModelXML("GameAssets\\MODEL\\707.xml");
	
	//p1->VSetShader(pShader2);
	//p1->PostInit();
	//p2->VSetShader(pShader);
	//p2->PostInit();
	//p3->VSetShader(pShader);
	//p3->PostInit();
	IEvent* pEvent = new EvtData_SetAnimation(p1->GetId(), sniper +idle,1);
	gEventManager()->VQueueEvent(pEvent);

	// tests
	//REGISTER_SCRIPT_EVENT(EvtData_ScriptEventTest_ToLua, EvtData_ScriptEventTest_ToLua::sk_EventType);

	//gLuaState()->VExecuteFile("GameAssets\\SCRIPTS\\test.lua");
	
	//EvtData_ScriptEventTest_ToLua* pEvent = new EvtData_ScriptEventTest_ToLua(33);
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

	ScriptExports::Unregister();
	LuaStateManager::shutDown();
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
	
	glPolygonMode(GL_BACK, GL_LINE);
	bool m_bUpdatePhysic = false;
	gTimer()->Reset();
	while (m_bRunMainLoop)
	{
		// Check Exit
		if (glfwWindowShouldClose(m_pWindow->Window())) m_bRunMainLoop = false;
		if (gInput()->KeyDown(DIK_ESCAPE)) glfwSetWindowShouldClose(m_pWindow->Window(), GLFW_TRUE);
		glfwPollEvents();

		
		gTimer()->Tick();

		gEventManager()->VUpdate(20);

		gInput()->Update();

		//gPhysic()->VClearForce(p3->GetId());
		
		
		/*if (gInput()->KeyDown(DIK_H))
		{
			gPhysic()->VSetVelocity( p3->GetId(), vec3(0.0, 0.0, -5.0));
		}
		if (gInput()->KeyDown(DIK_L))
		{
			gPhysic()->VStopActor(p3->GetId());
		}
		
		if (gInput()->KeyDown(DIK_G))
		{
			gPhysic()->VSetVelocity(p3->GetId(),vec3(1.0, 0.0, 0.0));
		}
		if (gInput()->KeyDown(DIK_J))
		{
			gPhysic()->VSetVelocity(p3->GetId(), vec3(-1.0, 0.0, 0.0));
		}*/
		gPhysic()->VOnUpdate(gTimer()->GetDeltaTime());
		//cout << gTimer()->GetDeltaTime() << endl;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0, 0.5, 0.1, 1.0f);

		//gPhysic()->VRenderDiagnostics();
		

		m_pScene->OnUpdate(gTimer()->GetDeltaTime());

		m_pScene->OnRender();
		


		glfwSwapBuffers(m_pWindow->Window());
	}

	return false;
}
