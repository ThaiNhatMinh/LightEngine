#include <stdafx.h>

#include "StateStack.h"
#include "GameState.h"
#include <Events.h>
#include <IEventManager.h>
#include <IEvent.h>
void StateStack::OnInputEvent(std::shared_ptr<Light::IEvent> pEvent)
{
	Light::events::EvtInput* pTemp = static_cast<Light::events::EvtInput*>(pEvent.get());
	for (auto el = m_Stacks.rbegin(); el != m_Stacks.rend(); el++)
		if (!(*el)->OnInputEvent(pTemp)) break;
}

StateStack::StateStack(Light::IContext *pContext):m_pContext(pContext)
{
	using namespace Light;
	m_pContext->GetSystem<Light::IEventManager>()->VAddListener(new EventDelegate<StateStack>(this, &StateStack::OnInputEvent), events::EvtInput::StaticType);
}

void StateStack::Update(float dt)
{
	for (auto el = m_Stacks.rbegin(); el != m_Stacks.rend(); el++)
		if (!(*el)->Update(dt)) break;

}

std::unique_ptr<GameState> StateStack::Pop()
{

	auto r = std::move(m_Stacks.back());
	m_Stacks.pop_back();
	r->OnExit(this);
	return std::move(r);
}

void StateStack::Push(GameState *pState)
{
	
	pState->OnEnter(this);
	m_Stacks.push_back(std::unique_ptr<GameState>(pState));
}

Light::IContext * StateStack::GetContext()
{
	return m_pContext;
}
