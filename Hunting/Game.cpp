#include "stdafx.h"

#include <IResourceManager.h>
#include <IEventManager.h>
#include <IFactory.h>
#include <ITimer.h>
#include <iostream>
#include "Game.h"



const char *vertexShaderSource = "#version 140\n"
"in vec3 aPos;\n"
"in vec3 aColor;\n"
"out vec3 oColor;\n"
"void main(){\n"
"oColor = aColor;gl_Position = vec4(aPos,1.0);\n"
"}";
const char *pixelShaderSource = "#version 140\n"
"in vec3 oColor;out vec3 color;\n"
"uniform sampler2D uTex;\n"
"void main(){\n"
" color = oColor;\n"
"}";



void Game::Init(Light::IContext* pContext)
{
	std::cout << "Hello from DLL";
	using namespace Light;

	m_pRenderer = pContext->GetSystem<render::RenderDevice>();
	render::VertexShader *vertexShader = m_pRenderer->CreateVertexShader(vertexShaderSource);

	// fragment shader
	render::PixelShader *pixelShader = m_pRenderer->CreatePixelShader(pixelShaderSource);
	//render::Texture* pTex = a.LoadTexture("GameAssets\\TEXTURES\\grass_autumn_orn_d.jpg");
	// link shaders
	pipeline = m_pRenderer->CreatePipeline(vertexShader, pixelShader);
	//render::PipelineParam* param = pipeline->GetParam("uTex");
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0, 0.0, 1.0f,
		0.5f, -0.5f, 0.0f,	1.0, 0.0,0.5f,
		0.0f, 0.5f, 0.0f,	0.5,1.0,0.2f };

	vertexBuffer = m_pRenderer->CreateVertexBuffer(sizeof(vertices), vertices);

	render::VertexElement vertexElement[] = {
		{ render::SHADER_POSITION_ATTRIBUTE, render::VERTEXELEMENTTYPE_FLOAT, 3, sizeof(float) * 6, 0 },
		{ render::SHADER_COLOR_ATTRIBUTE, render::VERTEXELEMENTTYPE_FLOAT, 3, sizeof(float) * 6, 12 } };

	render::VertexDescription *vertexDescription = m_pRenderer->CreateVertexDescription(2, vertexElement);

	vertexArray = m_pRenderer->CreateVertexArray(1, &vertexBuffer, &vertexDescription);
	delete vertexShader;
	delete pixelShader;
	delete vertexDescription;
	//delete vertexBuffer;

	
	Light::resources::IResourceManager* pResource = pContext->GetSystem<Light::resources::IResourceManager>();
	//loading = pResource->VLoadResource("GameAssets\\Resources.xml",true);
	auto pEventManager = pContext->GetSystem<Light::IEventManager>();
	auto pFactory = pContext->GetSystem<Light::IFactory>();
	auto pTimer = pContext->GetSystem<Light::ITimer>();
}

void Game::Render()
{
	//if(loading && loading->status==loading->LOADING) std::cout << "Percent: " << loading->percent*100 << "%" << std::endl;
	m_pRenderer->SetPipeline(pipeline);
	//param->SetAsInt(0);
	//m_pRenderer->SetTexture(0, pTex);
	m_pRenderer->SetVertexArray(vertexArray);
	m_pRenderer->Draw(0, 3);
}

void Game::ShutDown()
{
	delete vertexArray;
	delete pipeline;
	delete vertexBuffer;
}