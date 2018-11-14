#pragma once

#include <IGamePlugin.h>
#include <IContext.h>
#include <IScene.h>
#include "GameState/StateStack.h"
class Game : public IGamePlugin
{
public:
	// Call after engine init
	virtual void	Init(Light::IContext* pContext);
	// Call every frame
	virtual void	Update(float dt);
	// Call before engine shutdown
	virtual void	ShutDown();

private:
	//Light::IScene* pScene;
	std::unique_ptr<StateStack> m_StateStack;
	Light::render::RenderDevice* m_pRenderer;
	/*Light::render::Pipeline *pipeline;
	Light::render::VertexArray *vertexArray;
	Light::render::VertexBuffer *vertexBuffer;
	Light::render::Texture* pTex;*/
};