#include "stdafx.h"

AIExplosive::AIExplosive(ActorId id) :Zombie(id), m_State(IDLE)
{
}

AIExplosive::~AIExplosive()
{
}

void AIExplosive::TakeDamage(int damage)
{
	if (m_State == DEATH) return;

	m_State = BEING_ATTACKED;
	GetComponent<AnimationComponent>(AnimationComponent::Name)->Play(AnimationComponent::upper, M_BHit02);

	Zombie::TakeDamage(damage);
}

void AIExplosive::Death()
{
	m_State = DEATH;
	GetComponent<AnimationComponent>(AnimationComponent::Name)->Play(AnimationComponent::fullbody, M_Skill01);
	GetComponent<SoundSource3D>(SoundSource3D::Name)->Play("EXPLOSIONALARM");
	
}

HRESULT AIExplosive::VOnUpdate(Scene * pScene, float elapsedMs)
{
	Zombie::VOnUpdate(pScene, elapsedMs);

	if (m_State == DEATH)
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
	}
	return S_OK;
}

void AIExplosive::ExplosiveSkill()
{
	auto bomd = std::make_shared<EvtExplosion>(m_TransformComponent->GetPosition(),EX_DAMAGE,(float)EX_RANGE1, (float)EX_RANGE2);
	m_Context->GetSystem<EventManager>()->VQueueEvent(bomd);
}
