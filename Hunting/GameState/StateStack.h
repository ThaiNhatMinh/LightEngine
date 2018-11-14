#pragma once
#include <list>
#include <stack>
#include <memory>
#include <Events.h>
#include <IEvent.h>
#include <IContext.h>
#include "GameState.h"
class StateStack
{
public:
	friend class GameState;
private:
	std::list<std::unique_ptr<GameState>> m_Stacks;
	std::map<std::size_t, std::function<GameState*()>> m_StateFactory;

	Light::IContext* m_pContext;
private:
	void OnInputEvent(std::shared_ptr<Light::IEvent> pEvent);
public:
	StateStack() {};
	StateStack(Light::IContext*);
	void						Update(float dt);
	std::unique_ptr<GameState>	Pop();
	void						Push(GameState*);
	template<class T>void		RegisterState();
	Light::IContext*			GetContext();
};

template<class T>
void StateStack::RegisterState()
{
	auto func = [this]() {return new T(this); };

	m_StateFactory.insert(std::pair<std::size_t, std::function<GameState*()>>(typeid(T).hash_code(), func));
}
