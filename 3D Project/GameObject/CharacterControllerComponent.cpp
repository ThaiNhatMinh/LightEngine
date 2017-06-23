#include "pch.h"
#include "CharacterControllerComponent.h"

const char* CharacterControllerComponent::Name = "CharacterControllerComponent";


CharacterControllerComponent::CharacterControllerComponent():m_fJumpForce(0),m_fMaxSpeed(0),m_Direction(0,0,1),m_JumpDirection(0,1,0), m_bCanJump(0)
{
}

CharacterControllerComponent::~CharacterControllerComponent()
{
	gEventManager()->VRemoveListener(MakeDelegate(this, &CharacterControllerComponent::PhysicCollisionEvent), EvtData_PhysCollision::sk_EventType);
}

bool CharacterControllerComponent::VInit(tinyxml2::XMLElement * pData)
{
	tinyxml2::XMLElement* pSpeedElement = pData->FirstChildElement("Speed");
	m_fMaxSpeed = pSpeedElement->DoubleAttribute("max", 1.0f);

	tinyxml2::XMLElement* pJumpElement = pData->FirstChildElement("Jump");
	m_fJumpForce = pJumpElement->DoubleAttribute("force", 1.0f);

	return true;

}

tinyxml2::XMLElement * CharacterControllerComponent::VGenerateXml(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

void CharacterControllerComponent::VPostInit(void)
{
	m_pTransformC = m_pOwner->GetComponent<TransformComponent>(TransformComponent::Name);
	gEventManager()->VAddListener(MakeDelegate(this, &CharacterControllerComponent::PhysicCollisionEvent), EvtData_PhysCollision::sk_EventType);
}

void CharacterControllerComponent::VUpdate(float dt)
{

	vec3 direction = vec3(0,0,0);
	if (gInput()->KeyDown(DIK_SPACE)&& m_bCanJump)
	{
		gPhysic()->VApplyForce(vec3(0.0, 1.0, 0.0), m_fJumpForce, m_pOwner->GetId());
		m_bCanJump = false;

	}

	if (gInput()->KeyDown(DIK_Y) && m_bCanJump) direction += m_pTransformC->GetFront();
	if (gInput()->KeyDown(DIK_H) && m_bCanJump) direction -= m_pTransformC->GetFront();
	if (gInput()->KeyDown(DIK_G) && m_bCanJump) direction += m_pTransformC->GetRight();
	if (gInput()->KeyDown(DIK_J) && m_bCanJump) direction -= m_pTransformC->GetRight();


	
	
	
	if(direction != vec3(0))
	{
		direction = m_fMaxSpeed * glm::normalize(direction);
		gPhysic()->VSetVelocity(m_pOwner->GetId(), direction);
	}else if(m_bCanJump) gPhysic()->VSetVelocity(m_pOwner->GetId(), vec3(0));
	
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
	const EvtData_PhysCollision* p = dynamic_cast<const EvtData_PhysCollision*>(pEvent);
	if (p->GetActorA() == m_pOwner->GetId() || p->GetActorB() == m_pOwner->GetId())
		m_bCanJump = true;

}
