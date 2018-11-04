#pragma once


#include "GameState.h"
#include <IScene.h>

class GS_Game: public GameState
{
public:
	GS_Game(StateStack* pOwner);

	virtual void OnEnter(StateStack * pOwner);
	virtual void OnExit(StateStack * pOwner);
	virtual bool Update(float dt);

private:
	Light::IScene* m_pScene;
};