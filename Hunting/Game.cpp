#include "stdafx.h"

#include <IResourceManager.h>
#include <IEventManager.h>
#include <IFactory.h>
#include <ITimer.h>
#include <Renderer.h>
#include <iostream>
#include "Game.h"
#include <Events.h>
#include "GameState/GS_Loading.h"


void Game::Init(Light::IContext* pContext)
{
	std::cout << "Hello from DLL\n";
	using namespace Light;
	m_StateStack.reset(new StateStack(pContext));
	m_StateStack->Push(new GS_Loading(m_StateStack.get()));

	

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
	//pScene->VOnUpdate(dt);
	m_StateStack->Update(dt);

}