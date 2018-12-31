#include "stdafx.h"
#include "GS_Game.h"
#include "StateStack.h"
#include <IFactory.h>
#include <Events.h>
#include <IEventManager.h>
#include <IRenderSystem.h>
GS_Game::GS_Game(StateStack * pOwner):m_pScene(nullptr)
{
	m_pContext = pOwner->GetContext();
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
	m_pWindow = pContext->GetSystem<Light::IWindow>();
	auto pResourceM = pContext->GetSystem<Light::resources::IResourceManager>();
	auto pRenderS = pContext->GetSystem<Light::render::IRenderSystem>();
	m_pInput = pContext->GetSystem<Light::IInput>();
	m_pWindow->HideMouse(1);

	pTex = pRenderS->VCreateTexture(pResourceM->VGetTexture(std::vector<std::string>{"GameAssets\\test\\AK_47_KNIFE_ROYALDRAGON3_BG.DTX"},false,true));

	m_pScene = pFactory->VCreateScene("GameAssets\\test\\Scene.xml");

	auto pFont = m_pVGUI->VCreateFont("UNISPACE BD.TTF",50);
	m_pVGUI->VSetDefaultFont(pFont);
	
	

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
	if (m_pInput->VOnKey(Light::Key::Escape)) m_pContext->VExit();
	m_pVGUI->VDrawText("AabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNPQRSTUVWXYZ0123456789",glm::vec2(100,400));
	m_pVGUI->VDraw(pTex, glm::vec2(0, 300));
	m_pScene->VOnUpdate(dt);

	return false;
}

bool GS_Game::OnInputEvent(Light::events::EvtInput * pEvent)
{
	return false;
}
