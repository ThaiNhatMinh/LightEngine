#include "stdafx.h"
#include "Game.h"

void Game::Init(Context *c)
{
	m_Scene = std::unique_ptr<Scene>(new Scene(c));
	ActorFactory& factory = m_Scene->GetActorFactory();

	Actor* p4 = factory.CreateActor("GameAssets\\ACTOR\\Player.xml", nullptr, nullptr);
	m_Scene->GetRoot()->VAddChild(std::unique_ptr<Actor>(p4));

	Actor*pp = factory.CreateActor("GameAssets\\ACTOR\\Terrain.xml", nullptr, nullptr);
	m_Scene->GetRoot()->VAddChild(std::unique_ptr<Actor>(pp));
	pp = factory.CreateActor("GameAssets\\ACTOR\\PV.xml", nullptr, nullptr);
	m_Scene->GetRoot()->VAddChild(std::unique_ptr<Actor>(pp));

}

void Game::Update(float dt)
{
	m_Scene->OnUpdate(dt);
}

void Game::Render()
{
	m_Scene->OnRender();
}
