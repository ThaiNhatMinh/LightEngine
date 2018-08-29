#include "stdafx.h"

#include <IResourceManager.h>
#include <IEventManager.h>
#include <IFactory.h>
#include <ITimer.h>
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

	Light::IScene* pScene = Factory->VCreateScene("Test");
	pScene->VLoad("GameAssets\\test\\Scene.xml");
	//render::VertexShader *vertexShader = pResources->VGetVertexShader("GameAssets\\test\\test.vs");
	//// fragment shader
	//render::PixelShader *pixelShader = pResources->VGetPixelShader("GameAssets\\test\\test.fs");
	//pTex = pResources->VGetTexture("GameAssets\\TEXTURES\\grass_autumn_orn_d.jpg");
	//render::Model* pModel = pResources->VGetModel("GameAssets\\test\\Crate1.obj");
	//// link shaders
	//pipeline = m_pRenderer->CreatePipeline(vertexShader, pixelShader);
	//float vertices[] = {
	//	-0.5f, -0.5f, 0.0f, 0.0, 0.0,
	//	0.5f, -0.5f, 0.0f,	1.0, 0.0,
	//	0.0f, 0.5f, 0.0f,	0.5,1.0};

	//vertexBuffer = m_pRenderer->CreateVertexBuffer(sizeof(vertices), vertices);

	//render::VertexElement vertexElement[] = {
	//	{ render::SHADER_POSITION_ATTRIBUTE, render::VERTEXELEMENTTYPE_FLOAT, 3, sizeof(float) * 5, 0 },
	//	{ render::SHADER_TEXCOORD_ATTRIBUTE, render::VERTEXELEMENTTYPE_FLOAT, 2, sizeof(float) * 5, 12 } };

	//render::VertexDescription *vertexDescription = m_pRenderer->CreateVertexDescription(2, vertexElement);

	//vertexArray = m_pRenderer->CreateVertexArray(1, &vertexBuffer, &vertexDescription);
	//
	//delete vertexDescription;

	
	//loading = pResource->VLoadResource("GameAssets\\Resources.xml",true);
	auto pEventManager = pContext->GetSystem<Light::IEventManager>();
	auto pFactory = pContext->GetSystem<Light::IFactory>();
	auto pTimer = pContext->GetSystem<Light::ITimer>();
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