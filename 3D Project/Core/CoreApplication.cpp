#include "..\pch.h"


void CoreApplication::onStartUp()
{
	E_DEBUG("Application StartUp...");
	m_pWindow = new Windows("Ex-T1", 800, 600);
	m_pWindow->InitWindow();
	DirectInput::startUp(m_pWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	ActorFactory::startUp();
	GameTimer::startUp();
	Resources::startUp();

	m_pScene = new Scene();
	Camera* cam = new Camera(vec3(0,0,20),vec3(0,0,0),vec3(0,1,0));
	Frustum* frustum = new Frustum(52.0f, 4.0f / 3.0f, 1.0f, 1000.0f);
	m_pScene->SetCamera(cam);
	m_pScene->SetFrustum(frustum);

	Actor* p1 = gActorFactory()->CreateActor("Cube", CUBE, Math::g_Indentity);
	Actor* p2 = gActorFactory()->CreateActor("GameAssets\\player_teapot.xml",nullptr,nullptr);
	m_pScene->GetRoot()->VAddChild(p1);
	m_pScene->GetRoot()->VAddChild(p2);

	Shader* pShader = gResources()->LoadShader("NoTexture", "GameAssets\\SHADER\\NoTexture.vs", "GameAssets\\SHADER\\NoTexture.fs");
	Shader* pShader2 = gResources()->LoadShader("Texture", "GameAssets\\SHADER\\NoTexture.vs", "GameAssets\\SHADER\\Texture.fs");
	p1->VSetShader(pShader);
	p1->PostInit();
	p2->VSetShader(pShader2);
	p2->PostInit();
	
}

void CoreApplication::onShutDown()
{
	E_DEBUG("Application ShutDown...");
	DirectInput::shutDown();
	ActorFactory::shutDown();
	GameTimer::shutDown();
	Resources::shutDown();
	delete m_pWindow;

	
}

bool CoreApplication::MainLoop()
{

	m_bRunMainLoop = true;
	gTimer()->Reset();


	//CubeMesh* mesh = new CubeMesh;
	//Shader* pShader = gResources()->GetShader("NoTexture");
	//mesh->Finalize(pShader);

	while (m_bRunMainLoop)
	{
		gTimer()->Tick();
		gInput()->Update();
		glfwPollEvents();
		cout << gTimer()->GetFPS() << endl;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1, 0.1, 0.1, 1.0f);

		if (glfwWindowShouldClose(m_pWindow->Window())) m_bRunMainLoop = false;
		
		if (gInput()->KeyDown(DIK_ESCAPE)) glfwSetWindowShouldClose(m_pWindow->Window(), GLFW_TRUE);

		m_pScene->OnUpdate(gTimer()->GetDeltaTime());

		m_pScene->OnRender();
		
		/*mat4 MVP = m_pScene->GetViewProj();
		pShader->Use();
		pShader->SetUniformMatrix("Model", Math::g_Indentity.ToFloatPtr());
		pShader->SetUniformMatrix("MVP", MVP.ToFloatPtr());
		
		glBindVertexArray(mesh->VAO);
		glDrawElements(GL_TRIANGLES,36 , GL_UNSIGNED_INT, 0);
		*/

		glfwSwapBuffers(m_pWindow->Window());
	}

	return false;
}
