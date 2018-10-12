#include "stdafx.h"
#include "Game.h"

ZombieController::ZombieController():m_pTarget(nullptr),m_pGame(nullptr)
{
}

bool ZombieController::VInit(const tinyxml2::XMLElement * pData)
{
	m_pGame = m_Context->GetSystem<Game>();
	return true;
}

void ZombieController::VUpdate(float dt)
{
	if (!m_pTarget) FindTarget(dt);
	
}

const char * ZombieController::VGetName() const
{
	return Name;
}

bool ZombieController::CanMove()
{
	bool result = (m_pTarget != nullptr);
	return result;
}

void ZombieController::FindTarget(float dt)
{
	auto players = m_pGame->GetPlayerList();
	int i = int(dt*1000) % players.size();
	std::cout << "INDEX: " << i << std::endl;
	m_pTarget = players[i];
}
