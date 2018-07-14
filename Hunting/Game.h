#pragma once

#include <IGamePlugin.h>
#include <IContext.h>

class Game : public IGamePlugin
{
public:
	// Call after engine init
	virtual void	Init(Light::IContext* pContext);
	// Call every frame
	virtual void	Update(float dt) {};
	// Call every frame
	virtual void	Render();
	// Call before engine shutdown
	virtual void	ShutDown() {};

private:
	
	Light::render::RenderDevice* m_pRenderer;
	Light::render::Pipeline *pipeline;
	Light::render::VertexArray *vertexArray;

};