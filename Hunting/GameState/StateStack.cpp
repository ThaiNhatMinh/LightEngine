#include <stdafx.h>

#include "StateStack.h"
#include "GameState.h"
void StateStack::OnInputEvent(std::shared_ptr<Light::IEvent> pEvent)
{

}

StateStack::StateStack(Light::IContext *pContext):m_pContext(pContext)
{
}

void StateStack::Update(float dt)
{
	m_Stacks.top()->Update(dt);

}

std::unique_ptr<GameState> StateStack::Pop()
{

	auto r = std::move(m_Stacks.top());
	m_Stacks.pop();
	r->OnExit(this);
	return std::move(r);
}

void StateStack::Push(GameState *pState)
{
	pState->OnEnter(this);
	m_Stacks.push(std::unique_ptr<GameState>(pState));
}

Light::IContext * StateStack::GetContext()
{
	return m_pContext;
}
