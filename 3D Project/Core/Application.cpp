#include <pch.h>
#include "Application.h"
#include <type_traits>

#include "OpenGLWindows.h"
#include "EventManager.h"
#include "ActorFactory.h"
#include "Timer.h"
#include "Context.h"
#include "..\Graphics3D\OpenGL\OpenGLRenderDevice.h"
#include "..\Graphics3D\OpenGL\OpenGLRenderBuffer.h"
#include "..\Graphics3D\OpenGL\OpenGLTexture.h"
#include "..\Graphics3D\OpenGL\OpenGLFrameBuffer.h"
#include "..\ResourceManager\ResourceManager.h"
#include "..\Core\OpenGLInput.h"
#include "..\Graphics3D\RenderPass\OutlinePass.h"
using namespace Light;

void Application::SetupSubmodule()
{
	E_DEBUG("Application StartUp...");

	m_Context = std::unique_ptr<Light::IContext>(DEBUG_NEW Light::Context());
	
	m_pEventManager = std::unique_ptr<Light::IEventManager>(DEBUG_NEW Light::EventManager(m_Context.get()));

	m_pWindows = std::unique_ptr<Light::IWindow>(DEBUG_NEW Light::OpenGLWindows(m_Context.get()));
	auto a = DEBUG_NEW Light::render::OpenGLRenderDevice(m_Context.get());
	m_pRenderer = std::unique_ptr<Light::render::RenderDevice>(a);
	m_pResources = std::unique_ptr<IResourceManager>(DEBUG_NEW Light::resources::ResourceManager(m_Context.get()));



	m_pActorFactory = std::unique_ptr<Light::IFactory>(DEBUG_NEW Light::ActorFactory(m_Context.get()));
	//m_pSoundEngine = std::unique_ptr<SoundEngine>(DEBUG_NEW SoundEngine(m_Context.get()));
	
	//m_pSystemUI = std::unique_ptr<SystemUI>(DEBUG_NEW SystemUI(m_Context.get()));
	m_pInput = std::unique_ptr<Light::IInput>(DEBUG_NEW Light::OpenGLInput(m_Context.get()));
	//m_pConsole = std::unique_ptr<Console>(DEBUG_NEW Console(m_Context.get()));
	//m_pDebuger = std::unique_ptr<Debug>(DEBUG_NEW Debug(m_Context.get()));
	//m_pPhysic = std::unique_ptr<BulletPhysics>(DEBUG_NEW BulletPhysics(m_Context.get()));
	m_pTimer = std::unique_ptr<Light::ITimer>(DEBUG_NEW Light::GameTimer(m_Context.get()));
	//m_pEffectSystem = std::unique_ptr<EffectSystem>(DEBUG_NEW EffectSystem(m_Context.get()));
	//m_pVGUI = std::unique_ptr<VGUI>(DEBUG_NEW VGUI(m_Context.get()));
	
	
	//m_pConsole->RegisterVar("debug_physic", &m_DebugPhysic, 1, sizeof(int), TYPE_INT);
	//m_pConsole->RegisterVar("debug_hitbox", &m_Context->DrawSkeleton, 1, sizeof(int), TYPE_INT);
	//a->Test();

	render::OutlineRenderPass* pOutline = DEBUG_NEW render::OutlineRenderPass("Outline", m_Context.get());
	m_pRenderer->AddExtraPass(pOutline);

	m_pWindows->HideMouse(1);
}


Application::~Application()
{
	
}



void Application::MainLoop()
{
	
	SetupSubmodule();

	/*int w, h;
	m_pWindows->VGetWindowSize(w, h);
	
	render::OpenGLFrameBuffer framebuffer;
	framebuffer.Begin();

	render::TextureCreateInfo config;
	config.eTarget = render::TEXTURE_2D;
	config.eFormat = render::FORMAT_RGB;
	config.eType = render::UNSIGNED_BYTE;
	config.iInternalFormat = render::FORMAT_RGB;
	config.iLevel = 0;
	config.pData = nullptr;
	config.uiWidth = w;
	config.uiHeight = h;
	render::OpenGLTexture tex(config);
	framebuffer.AttachTexture(render::COLOR_ATTACHMENT, &tex, 0);

	render::OpenGLRenderBuffer renderbuffer(w, h, render::ColorFormat::FORMAT_DEPTH24_STENCIL8);
	framebuffer.AttachRenderBuffer(render::DEPTH_STENCIL_ATTACHMENT, &renderbuffer);
	framebuffer.End();

	std::string Path = "GameAssets\\System\\";
	auto shader = m_pRenderer->CreatePipeline(m_pResources->VGetVertexShader(Path + "Shader\\Screen.vs"), m_pResources->VGetPixelShader(Path + "Shader\\Screen.fs"));


	float vertices[] = {
		 -1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	render::VertexBuffer *vertexBuffer = m_pRenderer->CreateVertexBuffer(sizeof(vertices), vertices);

	render::VertexElement vertexElement[] = 
	{
		{  render::SHADER_POSITION_ATTRIBUTE, render::VERTEXELEMENTTYPE_FLOAT, 2, sizeof(float) * 4, 0, },
		{ render::SHADER_TEXCOORD_ATTRIBUTE, render::VERTEXELEMENTTYPE_FLOAT, 2, sizeof(float) * 4,sizeof(float) * 2, }
	};
	render::VertexDescription *vertexDescription = m_pRenderer->CreateVertexDescription(2, vertexElement);

	render::VertexArray *vertexArray = m_pRenderer->CreateVertexArray(1, &vertexBuffer, &vertexDescription);
*/
	m_bRunMainLoop = true;
	// PROBLEM: How every thing update ?
	// 1. Timer
	// 2. Event Manager
	// 3. Input 
	// 4. Physic
	// 5. 

	//Scene			*pScene = m_GamePlugins->GetScene();

	
	//m_Context->GetSystem<Windows>()->ShowWindows();

	m_pTimer->VReset();

	IGamePlugin* pGame = m_GamePlugins.LoadPlugin();

	pGame->Init(m_Context.get());


	while (m_bRunMainLoop)
	{
		
		m_pInput->VUpdate();
		m_pTimer->VTick();
		// Update input
		//		m_pInput->VUpdate();
		if (m_pInput->VOnKey(Light::Escape))	m_bRunMainLoop = false;
		// Timer
		
		//m_pSystemUI->NewFrame();

		//ImGui::Text("FPS: %d", m_pTimer->GetFPS());
		// check if in console then don't update game
		//if (!m_pConsole->CheckStatus())
		//{

		float dt = m_pTimer->VGetDeltaTime();
		//	// Update Event
		m_pEventManager->VUpdate(20);
		//	// Update Game
		//	m_GamePlugins->UpdateGame(dt);
		//	// Update Physic
		//	m_pPhysic->VOnUpdate(dt);
		//	// Update Effect
		//	m_pEffectSystem->Update(pScene,dt);
		//	// Update Object
		//	m_pPhysic->VSyncVisibleScene();
		//	// Update sound
		//	m_pSoundEngine->Update();
		//	// Update Debuger
		//	m_pDebuger->Update();
		//	// Update VGUI
		//	m_pVGUI->Update(dt);
		//}

		//
		//if (m_DebugPhysic) m_pPhysic->VRenderDiagnostics();
		pGame->Update(dt);
		//pGame->Render();
		// draw our first triangle

		//framebuffer.Begin();
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		m_pRenderer->Render();
		//framebuffer.End();
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glDisable(GL_DEPTH_TEST);
		//m_pRenderer->Clear();
		//
		//m_pRenderer->SetPipeline(shader);
		//m_pRenderer->SetTexture(0, &tex);
		////glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		//
		//m_pRenderer->SetVertexArray(vertexArray);
		//m_pRenderer->Draw(0, 6);
		// Draw Game
		//m_GamePlugins->RenderGame();
		// Draw Effect
		//m_pEffectSystem->Render(pScene);
		// Draw Console
		//m_pConsole->Draw();
		// Daw Debug
		//m_pDebuger->Render(pScene);
		// Draw SystemUI
		//m_pSystemUI->Render();
		// Draw VGUI
		//m_pVGUI->Render();

		//m_pRenderer->SwapBuffer();

		m_pWindows->VSwapBuffer();


	}

	pGame->ShutDown();
	Log::OutputFile();
}