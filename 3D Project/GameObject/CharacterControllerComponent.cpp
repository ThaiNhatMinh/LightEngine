#include "pch.h"
#include "CharacterControllerComponent.h"

const char* CharacterControllerComponent::Name = "CharacterControllerComponent";


CharacterControllerComponent::CharacterControllerComponent():m_fJumpForce(0),m_fMaxSpeed(0),m_MoveDirection(0,0,0),m_JumpDirection(0,1,0),m_bOnGround(false),m_fInAirTime(0)
{
}

CharacterControllerComponent::~CharacterControllerComponent()
{
	m_Context->m_pEventManager->VRemoveListener(MakeDelegate(this, &CharacterControllerComponent::PhysicCollisionEvent), EvtData_PhysCollisionStart::sk_EventType);
	m_Context->m_pEventManager->VRemoveListener(MakeDelegate(this, &CharacterControllerComponent::PhysicPreStepEvent), EvtData_PhysPreStep::sk_EventType);
	m_Context->m_pEventManager->VRemoveListener(MakeDelegate(this, &CharacterControllerComponent::PhysicPostStepEvent), EvtData_PhysPostStep::sk_EventType);
}

bool CharacterControllerComponent::VInit(const tinyxml2::XMLElement* pData)
{
	if (!pData) return false;
	const tinyxml2::XMLElement* pSpeedElement = pData->FirstChildElement("Speed");
	
	m_fMaxSpeed = pSpeedElement->DoubleAttribute("max", 1.0f);

	const tinyxml2::XMLElement* pJumpElement = pData->FirstChildElement("Jump");
	m_fJumpForce = pJumpElement->DoubleAttribute("force", 100.0f);

	const tinyxml2::XMLElement* pMoveElement = pData->FirstChildElement("Move");
	m_fMoveForce = pMoveElement->DoubleAttribute("force", 100.0f);
	m_fBrakeForce = pMoveElement->DoubleAttribute("brakeforce", 30.0f);
	return true;

}

tinyxml2::XMLElement * CharacterControllerComponent::VGenerateXml(tinyxml2::XMLDocument * p)
{
	if (!p) return nullptr;
	tinyxml2::XMLElement* pBaseElement = p->NewElement(VGetName());

	// initial transform -> position
	tinyxml2::XMLElement* pSpeedElement = p->NewElement("Speed");
	pSpeedElement->SetAttribute("max", m_fMaxSpeed);
	pBaseElement->LinkEndChild(pSpeedElement);


	tinyxml2::XMLElement* pJumpElement = p->NewElement("Jump");
	pJumpElement->SetAttribute("force", m_fJumpForce);
	pBaseElement->LinkEndChild(pJumpElement);

	tinyxml2::XMLElement* pMoveElement = p->NewElement("Move");
	pMoveElement->SetAttribute("force", m_fMoveForce);
	pMoveElement->SetAttribute("brakeforce", m_fBrakeForce);
	pBaseElement->LinkEndChild(pMoveElement);

	return pBaseElement;
	
}

void CharacterControllerComponent::VPostInit(void)
{
	// Get Transform component
	m_pTC = m_pOwner->GetTransform();

	// register event
	m_Context->m_pEventManager->VAddListener(MakeDelegate(this, &CharacterControllerComponent::PhysicCollisionEvent), EvtData_PhysOnCollision::sk_EventType);
	m_Context->m_pEventManager->VAddListener(MakeDelegate(this, &CharacterControllerComponent::PhysicPreStepEvent), EvtData_PhysPreStep::sk_EventType);
	m_Context->m_pEventManager->VAddListener(MakeDelegate(this, &CharacterControllerComponent::PhysicPostStepEvent), EvtData_PhysPostStep::sk_EventType);
	

	// Get Rigidbody
	m_pRBC = m_pOwner->GetComponent<RigidBodyComponent>(RigidBodyComponent::Name);
	m_pBAC = m_pOwner->GetComponent<AnimationComponent>(AnimationComponent::Name);
	//m_pRB->Activate(DISABLE_DEACTIVATION);
	
}

void CharacterControllerComponent::VUpdate(float dt)
{
	m_MoveDirection = vec3(0);
	m_JumpDirection = vec3(0);

	/*if (m_Context->m_pInput->KeyDown(DIK_Y))
	{
		m_MoveDirection -= m_pTC->GetFront();
		m_pBAC->PlayAnimation(run);
	}
	else if (m_Context->m_pInput->KeyDown(DIK_H))
	{
		m_MoveDirection += m_pTC->GetFront();
		m_pBAC->PlayAnimation(runBside);
	}
	else if (m_Context->m_pInput->KeyDown(DIK_G))
	{
		m_MoveDirection += m_pTC->GetRight();
		m_pBAC->PlayAnimation(runRside);
	}
	else if (m_Context->m_pInput->KeyDown(DIK_J))
	{
		m_MoveDirection -= m_pTC->GetRight();
		m_pBAC->PlayAnimation(runLside);
	}
	if (m_Context->m_pInput->KeyDown(DIK_SPACE)) m_JumpDirection = vec3(0, 1, 0);

	if (m_MoveDirection == vec3(0))
	{
		m_pBAC->PlayDefaultAnimation();
	}*/
	//m_MoveDirection = vec3(0);
	
}

void CharacterControllerComponent::VOnChanged(void)
{
}

const char * CharacterControllerComponent::VGetName() const
{
	return Name;
}

void CharacterControllerComponent::PhysicCollisionEvent(std::shared_ptr<IEvent> pEvent)
{
	const EvtData_PhysCollisionStart* p = static_cast<const EvtData_PhysCollisionStart*>(pEvent.get());
	if (p->GetActorA()->GetId() == m_pOwner->GetId() || p->GetActorB()->GetId() == m_pOwner->GetId())
		m_bOnGround = true;

}

void CharacterControllerComponent::PhysicPreStepEvent(std::shared_ptr<IEvent> pEvent)
{
	
	const EvtData_PhysPreStep *p = static_cast<const EvtData_PhysPreStep*>(pEvent.get());
	if (!m_bOnGround)
		m_fInAirTime += p->GetTimeStep();
	else m_fInAirTime = 0.0f;

	const float INAIR_THRESHOLD_TIME = 0.1f;
	const float INAIR_MOVE_FORCE = 0.02f;
	const float MOVE_FORCE = 100.0f;
	const float BRAKE_FORCE = 50.0f;
	
	bool softGrounded = m_fInAirTime < INAIR_THRESHOLD_TIME;
	const vec3& v = m_pRBC->GetLinearVelocity();

	// Velocity on the XZ plane
	vec3 planeVelocity(v.x, 0.0f, v.z);

	

	if (m_MoveDirection != vec3(0)&& m_bOnGround)
	{
	
		m_MoveDirection = glm::normalize(m_MoveDirection);
		m_pRBC->ApplyImpulse(m_MoveDirection *m_fMoveForce);
	}
	
	if (m_bOnGround)
	{
		vec3 brakeForce = -planeVelocity * m_fBrakeForce;
		m_pRBC->ApplyImpulse(brakeForce);
		
	}

	if (m_JumpDirection!=vec3(0)&& m_bOnGround)
	{
		
		m_pRBC->ApplyImpulse(vec3(0,1,0)*m_fJumpForce);
		
	}

	m_bOnGround = false;
}

void CharacterControllerComponent::PhysicPostStepEvent(std::shared_ptr<IEvent> pEvent)
{
	
}
