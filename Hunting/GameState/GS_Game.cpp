#include "stdafx.h"
#include "GS_Game.h"
#include "StateStack.h"
#include <IFactory.h>
GS_Game::GS_Game(StateStack * pOwner):m_pScene(nullptr)
{
	
}

void GS_Game::OnEnter(StateStack * pOwner)
{
	auto pContext = pOwner->GetContext();
	auto pFactory = pContext->GetSystem<Light::IFactory>();
	m_pScene = pFactory->VCreateScene("GameAssets\\test\\Scene.xml");
}

void GS_Game::OnExit(StateStack * pOwner)
{
}

bool GS_Game::Update(float dt)
{
	m_pScene->VOnUpdate(dt);
	return false;
}
