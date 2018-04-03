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
	State<Enity>* m_pCurrentState;
	State<Enity>* m_pPreviousState;
	State<Enity>* m_pGlobalState;

public:
	StateMachine(Enity* owner) :m_pOwner(owner), m_pCurrentState(nullptr), m_pGlobalState(nullptr), m), m_pPreviousState(nullptr){};
	virtual ~StateMachine() = default;

	void SetCurrentState(State<Enity>* currentState) {
		m_pCurrentState = currentState;
	};
	void SetGlobalState(State<Enity>* globalState)
	{
		m_pGlobalState = globalState;
	};
	void SetPrevState(State<Enity>* s)
	{
		m_pPreviousState = s;
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

		m_pCurrentState = pNewState;

		m_pCurrentState->Enter(m_pOwner);
	}

	void RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	};

	State<Enity>* CurrentState() { return m_pCurrentState; };
	State<Enity>* GlobalState() { return m_pGlobalState; };
	State<Enity>* PrevState() { return m_pPreviousState; };

	bool IsInState(const State<Enity>& s)const;
};