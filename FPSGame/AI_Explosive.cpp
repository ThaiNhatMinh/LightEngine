#include "stdafx.h"

AIExplosive::AIExplosive(ActorId id) :Zombie(id)/*, m_State(IDLE)*/, m_pStateMachine(this)
{
	m_pStateMachine.SetCurrentState(new ExplosiveIdle());
}

AIExplosive::~AIExplosive()
{
}

void AIExplosive::TakeDamage(int damage)
{
	if (!CanTakeDamage()) return;

	//m_State = BEING_ATTACKED;
	GetComponent<AnimationComponent>(AnimationComponent::Name)->Play(AnimationComponent::upper, M_BHit02);

	Zombie::TakeDamage(damage);
}

void AIExplosive::Death()
{
	//m_State = DEATH;
	//GetComponent<AnimationComponent>(AnimationComponent::Name)->Play(AnimationComponent::fullbody, M_Skill01);
	//GetComponent<SoundSource3D>(SoundSource3D::Name)->Play("EXPLOSIONALARM");
	m_pStateMachine.ChangeState(new ExplosiveDeath());
}

HRESULT AIExplosive::VOnUpdate(Scene * pScene, float elapsedMs)
{
	Zombie::VOnUpdate(pScene, elapsedMs);

	/*if (m_State == DEATH)
	{
		if (GetComponent<AnimationComponent>(AnimationComponent::Name)->IsFinish())
		{
			GetComponent<SoundSource3D>(SoundSource3D::Name)->Play("EXPLOSIONBOOM");
			ExplosiveSkill();
			Zombie::Death();
		}
	}
	else if (m_State == BEING_ATTACKED)
	{
		if (GetComponent<AnimationComponent>(AnimationComponent::Name)->IsFinish())
		{
			m_State = IDLE;
			GetComponent<AnimationComponent>(AnimationComponent::Name)->Play(AnimationComponent::fullbody, M_Idle);
		}
	}*/

	m_pStateMachine.Update(elapsedMs);

	return S_OK;
}

void AIExplosive::ExplosiveSkill()
{
	auto bomd = std::make_shared<EvtExplosion>(m_TransformComponent->GetPosition(),EX_DAMAGE,(float)EX_RANGE1, (float)EX_RANGE2);
	m_Context->GetSystem<EventManager>()->VQueueEvent(bomd);
}



void ExplosiveIdle::Enter(AIExplosive *pZombie)
{
	pZombie->GetComponent<AnimationComponent>(AnimationComponent::Name)->Play(AnimationComponent::fullbody, AIExplosive::M_Idle,true);
}

void ExplosiveIdle::Execute(AIExplosive *pZombie)
{
	// [To Do] Check if player attack then follow and revenger. LOL of course
	// Check if there is player near then attack. We stronger. =)) 

}

void ExplosiveDeath::Enter(AIExplosive *pZombie)
{
	m_pAnimComponent = pZombie->GetComponent<AnimationComponent>(AnimationComponent::Name);
	m_pSoundComponent = pZombie->GetComponent<SoundSource3D>(SoundSource3D::Name);

	m_pAnimComponent->Play(AnimationComponent::fullbody, AIExplosive::M_Skill01);
	m_pSoundComponent->Play("EXPLOSIONALARM");

	// zombie already death so it can't take damage anymore
	pZombie->m_bCanTakeDamage = false;

}

void ExplosiveDeath::Execute(AIExplosive *pZombie)
{

	if (m_pAnimComponent->IsFinish())
	{
		m_pSoundComponent->Play("EXPLOSIONBOOM");
		pZombie->ExplosiveSkill();
		pZombie->VGetParent()->VRemoveChild(pZombie->GetId());
	}
}

void ExplosiveRunning::Execute(AIExplosive * pZombie)
{
	// [To do] find path to enemy
}
