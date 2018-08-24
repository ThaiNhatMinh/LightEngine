#include "stdafx.h"

AIExplosive::AIExplosive(ActorId id) :Zombie(id)/*, m_State(IDLE)*/, StateMachine(this)
{
	StateMachine::SetCurrentState(new ExplosiveIdle());
}

AIExplosive::~AIExplosive()
{
}

void AIExplosive::TakeDamage(int damage)
{
	if (!CanTakeDamage()) return;

	
	GetComponent<AnimationComponent>(AnimationComponent::Name)->Play(AnimationComponent::upper, M_BHit02);

	Zombie::TakeDamage(damage);
}

void AIExplosive::Death()
{
	StateMachine::ChangeState(new ExplosiveDeath());
}

HRESULT AIExplosive::VOnUpdate(Scene * pScene, float elapsedMs)
{
	Zombie::VOnUpdate(pScene, elapsedMs);

	StateMachine::Update(elapsedMs);

	return S_OK;
}

void AIExplosive::ExplosiveSkill()
{
	auto bomd = std::make_shared<EvtExplosion>(m_TransformComponent->GetPosition(),EX_DAMAGE,(float)EX_RANGE1, (float)EX_RANGE2);
	m_Context->GetSystem<EventManager>()->VQueueEvent(bomd);
}



void ExplosiveIdle::Enter(AIExplosive *pZombie)
{
	m_pController = nullptr;
	pZombie->GetComponent<AnimationComponent>(AnimationComponent::Name)->Play(AnimationComponent::fullbody, AIExplosive::M_Idle,true);
	m_pController = pZombie->GetComponent<ZombieController>(ZombieController::Name);
}

void ExplosiveIdle::Execute(AIExplosive *pZombie)
{
	// [To Do] Check if player attack then follow and revenger. LOL of course
	// Check if there is player near then attack. We stronger. =)) 
	if (m_pController == nullptr) return;

	if (m_pController->CanMove()) pZombie->SetCurrentState(new ExplosiveRunning());
}

void ExplosiveRunning::Enter(AIExplosive *pZombie)
{
	m_pController = pZombie->GetComponent<ZombieController>(ZombieController::Name);
}

void ExplosiveRunning::Execute(AIExplosive * pZombie)
{
	// [To do] find path to enemy
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
