#pragma once
#include <assert.h>

template<class Enity = Actor>
class State
{
public:
	virtual ~State() = default;

	virtual void Enter(Enity*) = 0;
	virtual void Execute(Enity*) = 0;
	virtual void Exit(Enity*) = 0;
};

template<class Enity = Actor>
class StateMachine
{
private:
	Enity * m_pOwner;
	std::shared_ptr<State<Enity>> m_pCurrentState;
	std::shared_ptr<State<Enity>> m_pPreviousState;
	std::shared_ptr<State<Enity>> m_pGlobalState;

public:
	StateMachine(Enity* owner) :m_pOwner(owner), m_pCurrentState(nullptr), m_pGlobalState(nullptr), m_pPreviousState(nullptr){};
	virtual ~StateMachine() = default;

	void SetCurrentState(State<Enity>* currentState) {
		m_pCurrentState = std::shared_ptr<State<Enity>>(currentState);
	};
	void SetGlobalState(State<Enity>* globalState)
	{
		m_pGlobalState = std::shared_ptr<State<Enity>>(globalState);
	};
	void SetPrevState(State<Enity>* s)
	{
		m_pPreviousState = std::shared_ptr<State<Enity>>(s);
	}

	void Update(float dt)
	{
		if (m_pGlobalState) m_pGlobalState->Execute(m_pOwner);

		if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
	}

	void ChangeState(State<Enity>* pNewState)
	{
		assert(pNewState &&	"<StateMachine::ChangeState>: trying to change to a null state");

		m_pPreviousState = m_pCurrentState;

		m_pCurrentState->Exit(m_pOwner);

		m_pCurrentState = std::shared_ptr<State<Enity>>(pNewState);

		m_pCurrentState->Enter(m_pOwner);
	}

	void RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	};

	std::shared_ptr<State<Enity>> CurrentState() { return m_pCurrentState; };
	std::shared_ptr<State<Enity>> GlobalState() { return m_pGlobalState; };
	std::shared_ptr<State<Enity>> PrevState() { return m_pPreviousState; };

	bool IsInState(const State<Enity>& s)const {
		return 0;
	};
};