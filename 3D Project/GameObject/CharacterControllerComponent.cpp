#include "pch.h"
#include "CharacterControllerComponent.h"

const char* CharacterControllerComponent::Name = "CharacterControllerComponent";


CharacterControllerComponent::CharacterControllerComponent():m_fJumpForce(0),m_fMaxSpeed(0),m_MoveDirection(0,0,0),m_JumpDirection(0,1,0),m_bOnGround(false),m_fInAirTime(0)
{
}

CharacterControllerComponent::~CharacterControllerComponent()
{
	gEventManager()->VRemoveListener(MakeDelegate(this, &CharacterControllerComponent::PhysicCollisionEvent), EvtData_PhysCollisionStart::sk_EventType);
	gEventManager()->VRemoveListener(MakeDelegate(this, &CharacterControllerComponent::PhysicPreStepEvent), EvtData_PhysPreStep::sk_EventType);
	gEventManager()->VRemoveListener(MakeDelegate(this, &CharacterControllerComponent::PhysicPostStepEvent), EvtData_PhysPostStep::sk_EventType);
}

bool CharacterControllerComponent::VInit(const tinyxml2::XMLElement* pData)
{
	if (!pData) return false;
	const tinyxml2::XMLElement* pSpeedElement = pData->FirstChildElement("Speed");
	m_fMaxSpeed = pSpeedElement->DoubleAttribute("max", 1.0f);

	const tinyxml2::XMLElement* pJumpElement = pData->FirstChildElement("Jump");
	m_fJumpForce = pJumpElement->DoubleAttribute("force", 1.0f);

	return true;

}

tinyxml2::XMLElement * CharacterControllerComponent::VGenerateXml(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

void CharacterControllerComponent::VPostInit(void)
{
	// Get Transform component
	m_pTransformC = m_pOwner->GetComponent<TransformComponent>(TransformComponent::Name);

	// register event
	gEventManager()->VAddListener(MakeDelegate(this, &CharacterControllerComponent::PhysicCollisionEvent), EvtData_PhysCollisionStart::sk_EventType);
	gEventManager()->VAddListener(MakeDelegate(this, &CharacterControllerComponent::PhysicPreStepEvent), EvtData_PhysPreStep::sk_EventType);
	gEventManager()->VAddListener(MakeDelegate(this, &CharacterControllerComponent::PhysicPostStepEvent), EvtData_PhysPostStep::sk_EventType);
	

	// Get Rigidbody
	//BulletPhysics* pPhysic = (BulletPhysics*)gPhysic();
	m_pRB = m_pOwner->GetComponent<RigidBodyComponent>(RigidBodyComponent::Name);
	m_pRB->SetAngularFactor(vec3(0, 0, 0));
	m_pRB->SetFriction(10.0f);
	//m_pRB->Activate(DISABLE_DEACTIVATION);
	
}

void CharacterControllerComponent::VUpdate(float dt)
{
	m_MoveDirection = vec3(0);
	m_JumpDirection = vec3(0);
	if (gInput()->KeyDown(DIK_Y)) m_MoveDirection += m_pTransformC->GetFront();
	if (gInput()->KeyDown(DIK_H)) m_MoveDirection -= m_pTransformC->GetFront();
	if (gInput()->KeyDown(DIK_G)) m_MoveDirection += m_pTransformC->GetRight();
	if (gInput()->KeyDown(DIK_J)) m_MoveDirection -= m_pTransformC->GetRight();
	if (gInput()->KeyDown(DIK_SPACE)) m_JumpDirection = vec3(0, 1, 0);
	
}

void CharacterControllerComponent::VOnChanged(void)
{
}

const char * CharacterControllerComponent::VGetName() const
{
	return Name;
}

void CharacterControllerComponent::PhysicCollisionEvent(const IEvent * pEvent)
{
	const EvtData_PhysCollisionStart* p = static_cast<const EvtData_PhysCollisionStart*>(pEvent);
	if (p->GetActorA() == m_pOwner->GetId() || p->GetActorB() == m_pOwner->GetId())
		m_bOnGround = true;

}

void CharacterControllerComponent::PhysicPreStepEvent(const IEvent * pEvent)
{
	
	const EvtData_PhysPreStep *p = static_cast<const EvtData_PhysPreStep*>(pEvent);
	if (!m_bOnGround)
		m_fInAirTime += p->GetTimeStep();
	else m_fInAirTime = 0.0f;

	const float INAIR_THRESHOLD_TIME = 0.1f;
	const float INAIR_MOVE_FORCE = 0.02f;
	const float MOVE_FORCE = 100.0f;
	const float BRAKE_FORCE = 1.5f;
	// When character has been in air less than 1/10 second, it's still interpreted as being on ground
	bool softGrounded = m_fInAirTime < INAIR_THRESHOLD_TIME;
	const vec3& v = m_pRB->GetLinearVelocity();
	if (m_MoveDirection != vec3(0))
	{
		m_MoveDirection = glm::normalize(m_MoveDirection);
		m_pRB->ApplyImpulse(m_MoveDirection *MOVE_FORCE);
	}

	
}

void CharacterControllerComponent::PhysicPostStepEvent(const IEvent * pEvent)
{
	vec3 v = m_pRB->GetLinearVelocity();
	float l = glm::length(v);
	if (l > 20.0f && m_bOnGround)
	{
		v *= 2.0f / l;
		m_pRB->SetLinearVelocity(v);
	}
}
