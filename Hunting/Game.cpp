#include "stdafx.h"

#include <IResourceManager.h>
#include <IEventManager.h>
#include <IFactory.h>
#include <ITimer.h>
#include <Renderer.h>
#include <iostream>
#include "Game.h"
#include <Events.h>



void Game::Init(Light::IContext* pContext)
{
	std::cout << "Hello from DLL\n";
	using namespace Light;

	//m_pRenderer = pContext->GetSystem<render::RenderDevice>();
	auto pResources = pContext->GetSystem<resources::IResourceManager>();
	auto Factory = pContext->GetSystem<IFactory>();
	auto EventManager = pContext->GetSystem<IEventManager>();
	//pResources->VGetModel("GameAssets\\MODELS\\707_BL.xml");

	pScene = Factory->VCreateScene("Test");
	pScene->VLoad("GameAssets\\test\\Scene.xml");

	auto pEventManager = pContext->GetSystem<Light::IEventManager>();
	auto pFactory = pContext->GetSystem<Light::IFactory>();
	auto pTimer = pContext->GetSystem<Light::ITimer>();

	auto pEvent = new events::EvtRequestCreateSprite();
	pEvent->File = "GameAssets\\TEXTURES\\FX\\BLOOD3\\BLOOD3.SPR";
	pEvent->life = 10;
	pEvent->Pos = glm::vec3(0, 200, 0);
	pEvent->Parent = nullptr;
	EventManager->VQueueEvent(std::shared_ptr<IEvent>(pEvent));

	/*render::BlendConfig config;
	config.Enable = true;
	auto a = m_pRenderer->CreateBlendingState(config);
	m_pRenderer->SetBlendingState(a);*/
	

}

void Game::ShutDown()
{	
}

void Game::Update(float dt)
{
	pScene->VOnUpdate(dt);

}