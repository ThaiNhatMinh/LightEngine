#include "stdafx.h"

#include <IResourceManager.h>
#include <IEventManager.h>
#include <IFactory.h>
#include <ITimer.h>
#include <Renderer.h>
#include <iostream>
#include "Game.h"



void Game::Init(Light::IContext* pContext)
{
	std::cout << "Hello from DLL";
	using namespace Light;

	m_pRenderer = pContext->GetSystem<render::RenderDevice>();
	auto pResources = pContext->GetSystem<resources::IResourceManager>();
	auto Factory = pContext->GetSystem<IFactory>();

	//pResources->VGetModel("GameAssets\\MODELS\\707_BL.xml");

	pScene = Factory->VCreateScene("Test");
	pScene->VLoad("GameAssets\\test\\Scene.xml");

	auto pEventManager = pContext->GetSystem<Light::IEventManager>();
	auto pFactory = pContext->GetSystem<Light::IFactory>();
	auto pTimer = pContext->GetSystem<Light::ITimer>();

	

	render::DepthStencilConfig config1;
	config1.DepthEnable = false;
	config1.FrontStencilCompare = render::COMPARE_NOTEQUAL;
	config1.FrontRef = 1;
	config1.FrontCompareMask = 0xFF;


}

void Game::Render()
{
	//m_pRenderer->SetPipeline(pipeline);
	//param->SetAsInt(0);
	//m_pRenderer->SetTexture(0, pTex);
	//m_pRenderer->SetVertexArray(vertexArray);
	//m_pRenderer->Draw(0, 3);
	//pScene->VOnRender
}

void Game::ShutDown()
{
	//delete vertexArray;
	//delete pipeline;
	//delete vertexBuffer;
	
}

void Game::Update(float dt)
{
	pScene->VOnUpdate(dt);

}