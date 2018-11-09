#include "stdafx.h"
#include "GS_Game.h"
#include "StateStack.h"
#include <IFactory.h>
#include <Events.h>
#include <IEventManager.h>
#include <IWindow.h>
#include <IRenderSystem.h>
GS_Game::GS_Game(StateStack * pOwner):m_pScene(nullptr)
{
	auto m_pContext = pOwner->GetContext();
	m_pVGUI = m_pContext->GetSystem<Light::vgui::IVGUI>();
}

Light::render::Texture* pTex;
void GS_Game::OnEnter(StateStack * pOwner)
{
	printf("Enter GS_Game...\n");
	using namespace Light;
	auto pContext = pOwner->GetContext();
	auto pFactory = pContext->GetSystem<Light::IFactory>();
	auto pEventManager = pContext->GetSystem<Light::IEventManager>();
	auto pWindows = pContext->GetSystem<Light::IWindow>();
	auto pResourceM = pContext->GetSystem<Light::resources::IResourceManager>();
	auto pRenderS = pContext->GetSystem<Light::render::IRenderSystem>();
	pWindows->HideMouse(1);

	pTex = pRenderS->VCreateTexture(pResourceM->VGetTexture(std::vector<std::string>{"GameAssets\\test\\AK_47_KNIFE_ROYALDRAGON3_BG.DTX"},false,true));

	m_pScene = pFactory->VCreateScene("GameAssets\\test\\Scene.xml");

	//m_pRenderer = pContext->GetSystem<render::RenderDevice>();
	//auto pResources = pContext->GetSystem<resources::IResourceManager>();
	//auto Factory = pContext->GetSystem<IFactory>();
	//auto EventManager = pContext->GetSystem<IEventManager>();
	//pResources->VGetModel("GameAssets\\MODELS\\707_BL.xml");

	//pScene = Factory->VCreateScene("GameAssets\\test\\Scene.xml");

	

	auto pEvent = new events::EvtRequestCreateSprite();
	pEvent->File = "GameAssets\\TEXTURES\\FX\\BLOOD3\\BLOOD3.SPR";
	pEvent->life = 10;
	pEvent->Pos = glm::vec3(0, 200, 0);
	pEvent->Parent = nullptr;
	pEventManager->VQueueEvent(std::shared_ptr<IEvent>(pEvent));
}

void GS_Game::OnExit(StateStack * pOwner)
{
}

bool GS_Game::Update(float dt)
{
	m_pVGUI->VDrawText("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNPQRSTUVWXYZ0123456789",glm::vec2(500,400));
	m_pVGUI->VDraw(pTex, glm::vec2(100, 300));
	m_pScene->VOnUpdate(dt);
	return false;
}
