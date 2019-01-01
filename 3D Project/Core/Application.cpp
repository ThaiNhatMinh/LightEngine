#include <pch.h>
#include "Application.h"
#include <type_traits>
#include "Events.h"
#include "OpenGLWindows.h"
#include "EventManager.h"
#include "ActorFactory.h"
#include "Context.h"
#include "SoundEngine.h"
#include "OpenGLInput.h"
#include "DebugRender.h"
#include "..\Graphics3D\EffectSystem.h"
#include "..\Script\LuaScriptExporter.h"
#include "..\VGUI\VGUI.h"
#include "..\Editor\OpenGLSysUI.h"

#include "..\ResourceManager\ResourceManager.h"

using namespace Light;

void Application::SetupSubmodule()
{
	srand(time(NULL));

	E_DEBUG("Application StartUp...");

	m_Context = std::unique_ptr<Light::Context>(DEBUG_NEW Light::Context());
	
	m_pEventManager = std::unique_ptr<Light::EventManager>(DEBUG_NEW Light::EventManager(m_Context.get()));

	m_pWindows = std::unique_ptr<Light::IWindow>(DEBUG_NEW Light::OpenGLWindows(m_Context.get()));
	m_pResources = std::unique_ptr<resources::IResourceManager>(DEBUG_NEW Light::resources::ResourceManager(m_Context.get()));
	m_pRenderer = std::unique_ptr<Light::render::RenderSystem>(DEBUG_NEW Light::render::RenderSystem(m_Context.get()));
	
	
	m_pActorFactory = std::unique_ptr<Light::IFactory>(DEBUG_NEW Light::ActorFactory(m_Context.get()));
	m_pSoundEngine = std::unique_ptr<SoundEngine>(DEBUG_NEW SoundEngine(m_Context.get()));
	
	
	m_pInput = std::unique_ptr<Light::OpenGLInput>(DEBUG_NEW Light::OpenGLInput(m_Context.get()));
	
	
	m_pPhysic = std::unique_ptr<physics::BulletPhysics>(DEBUG_NEW physics::BulletPhysics(m_Context.get()));
	m_pTimer = std::unique_ptr<Light::GameTimer>(DEBUG_NEW Light::GameTimer(m_Context.get()));
	
	m_pScriptManager = std::unique_ptr<Light::LuaScriptManager>(DEBUG_NEW Light::LuaScriptManager(m_Context.get()));
	

	m_pResources->PostInit();
	m_pRenderer->PostInit();
	m_pPhysic->PostInit();
	m_pActorFactory->PostInit();
	

	
}


Application::~Application()
{
	
}



void Application::MainLoop()
{
	
	SetupSubmodule();
	

	
	//
	//int w, h;
	//m_pWindows->VGetWindowSize(w, h);
	//
	//render::OpenGLFrameBuffer framebuffer;
	//framebuffer.Begin();
	//render::TextureCreateInfo config;
	//config.eTarget = render::TEXTURE_2D;
	//config.eFormat = render::FORMAT_RGB;
	//config.eType = render::UNSIGNED_BYTE;
	//config.iInternalFormat = render::FORMAT_RGB;
	//config.iLevel = 0;
	//config.pData = nullptr;
	//config.uiWidth = w;
	//config.uiHeight = h;
	//render::OpenGLTexture tex(config);
	//framebuffer.AttachTexture(render::COLOR_ATTACHMENT, &tex, 0);
	//config.iInternalFormat = render::FORMAT_DEPTH_COMPONENT;
	//config.eFormat = render::FORMAT_DEPTH_COMPONENT;
	//config.eType = render::FLOAT;
	//render::OpenGLTexture depthtex(config);
	//framebuffer.AttachTexture(render::DEPTH_ATTACHMENT, &depthtex, 0);
	////render::OpenGLRenderBuffer renderbuffer(w, h, render::ColorFormat::FORMAT_DEPTH_COMPONENT);
	////framebuffer.AttachRenderBuffer(render::DEPTH_ATTACHMENT, &renderbuffer);
	//framebuffer.End();
	//std::string Path = "GameAssets\\System\\";
	//auto shader = m_pRenderer->CreatePipeline(m_pResources->VGetVertexShader(Path + "Shader\\Screen.vs"), m_pResources->VGetPixelShader(Path + "Shader\\Screen.fs"));
	//auto uTex = shader->GetParam("uTex");
	//auto udepthTex = shader->GetParam("uDepthTex");
	//float vertices[] = {
	//	 -1.0f,  1.0f,  0.0f, 1.0f,
	//	-1.0f, -1.0f,  0.0f, 0.0f,
	//	 1.0f, -1.0f,  1.0f, 0.0f,
	//	-1.0f,  1.0f,  0.0f, 1.0f,
	//	 1.0f, -1.0f,  1.0f, 0.0f,
	//	 1.0f,  1.0f,  1.0f, 1.0f
	//};
	//render::VertexBuffer *vertexBuffer = m_pRenderer->CreateVertexBuffer(sizeof(vertices), vertices);
	//render::VertexElement vertexElement[] = 
	//{
	//	{  render::SHADER_POSITION_ATTRIBUTE, render::VERTEXELEMENTTYPE_FLOAT, 2, sizeof(float) * 4, 0, },
	//	{ render::SHADER_TEXCOORD_ATTRIBUTE, render::VERTEXELEMENTTYPE_FLOAT, 2, sizeof(float) * 4,sizeof(float) * 2, }
	//};
	//render::VertexDescription *vertexDescription = m_pRenderer->CreateVertexDescription(2, vertexElement);
	//render::VertexArray *vertexArray = m_pRenderer->CreateVertexArray(1, &vertexBuffer, &vertexDescription);


	vgui::VGUI vGUI(m_Context.get());
	OpenGLSysUI SysUI(m_Context.get());
	SysUI.PostInit();
	


	IGamePlugin* pGame = m_GamePlugins.LoadPlugin();

	pGame->Init(m_Context.get());

	m_pScriptManager->Start();

	
	m_pTimer->VReset();

	while (!m_Context->Exiting() && !m_pWindows->VShouldClose())
	{
		m_pRenderer->PreRender();
		m_pInput->VUpdate();
		m_pTimer->VTick();
		

		//if (m_pInput->VOnKey(Light::Escape))	m_bRunMainLoop = false;
		
		float dt = m_pTimer->VGetDeltaTime();
		
		m_pEventManager->VUpdate(200);
		//m_pSystemUI->Update(dt);
		m_pScriptManager->Update(dt);
		SysUI.Update(dt);
		pGame->Update(dt);

		m_pPhysic->VOnUpdate(dt);
		m_pPhysic->VSyncVisibleScene();
		m_pSoundEngine->VUpdate();
		m_pRenderer->Update(dt);
		//m_pPhysic->VRenderDiagnostics();
		//framebuffer.Begin();
		
	
		
		//m_pDebuger->Render();
		m_pRenderer->Render();
		
		vGUI.Render();
		SysUI.Render();
		//m_pSystemUI->Render();
		/*framebuffer.End();
		

		glDisable(GL_DEPTH_TEST);
		m_pRenderer->Clear();
		
		m_pRenderer->SetPipeline(shader);
		uTex->SetAsInt(render::UNIT_DEFAULT);
		m_pRenderer->SetTexture(render::UNIT_DEFAULT, &tex);
		udepthTex->SetAsInt(render::UNIT_DEPTH);
		m_pRenderer->SetTexture(render::UNIT_DEPTH, &depthtex);
		
		m_pRenderer->SetVertexArray(vertexArray);
		m_pRenderer->Draw(0, 6);*/
	
		m_pWindows->VSwapBuffer();


	}

	pGame->ShutDown();
	Log::OutputFile();
}