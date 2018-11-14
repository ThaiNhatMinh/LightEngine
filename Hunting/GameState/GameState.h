#pragma once
#include <Events.h>
class StateStack;
class GameState
{
public:
	virtual void OnEnter(StateStack * pOwner) = 0;
	virtual void OnExit(StateStack * pOwner) = 0;
	virtual bool Update(float dt)=0;
	virtual bool OnInputEvent(Light::events::EvtInput* pEvent) = 0;
private:
	StateStack* m_pOwner;
};