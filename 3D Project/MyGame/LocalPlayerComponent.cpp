#include <pch.h> 
#include "LocalPlayerComponent.h"

const char* LocalPlayerComponent::Name = "LocalPlayerComponent";


LocalPlayerComponent::LocalPlayerComponent() :m_fJumpForce(0), m_fMaxSpeed(0), m_MoveDirection(0, 0, 0), m_JumpDirection(0, 1, 0), m_bOnGround(false), m_fInAirTime(0)
, m_Shooting(0)
{
}

LocalPlayerComponent::~LocalPlayerComponent()
{
	m_Context->m_pEventManager->VRemoveListener(MakeDelegate(this, &LocalPlayerComponent::PhysicCollisionEvent), EvtData_PhysCollisionStart::sk_EventType);
	m_Context->m_pEventManager->VRemoveListener(MakeDelegate(this, &LocalPlayerComponent::PhysicPreStepEvent), EvtData_PhysPreStep::sk_EventType);
	m_Context->m_pEventManager->VRemoveListener(MakeDelegate(this, &LocalPlayerComponent::PhysicPostStepEvent), EvtData_PhysPostStep::sk_EventType);
}

bool LocalPlayerComponent::VInit(const tinyxml2::XMLElement* pData)
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

tinyxml2::XMLElement * LocalPlayerComponent::VGenerateXml(tinyxml2::XMLDocument * p)
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

void LocalPlayerComponent::VPostInit(void)
{
	// Get Transform component
	m_pTC = m_pOwner->GetTransform();

	// register event
	m_Context->m_pEventManager->VAddListener(MakeDelegate(this, &LocalPlayerComponent::PhysicCollisionEvent), EvtData_PhysOnCollision::sk_EventType);
	m_Context->m_pEventManager->VAddListener(MakeDelegate(this, &LocalPlayerComponent::PhysicPreStepEvent), EvtData_PhysPreStep::sk_EventType);
	m_Context->m_pEventManager->VAddListener(MakeDelegate(this, &LocalPlayerComponent::PhysicPostStepEvent), EvtData_PhysPostStep::sk_EventType);


	// Get Rigidbody
	m_pRBC = m_pOwner->GetComponent<RigidBodyComponent>(RigidBodyComponent::Name);
	m_pBAC = m_pOwner->GetComponent<AnimationComponent>(AnimationComponent::Name);
	//m_pRB->Activate(DISABLE_DEACTIVATION);

}

void LocalPlayerComponent::VUpdate(float dt)
{
	m_Context->m_pDebuger->DrawCoord(m_pOwner->VGetGlobalTransform());
	m_MoveDirection = vec3(0);
	m_JumpDirection = vec3(0);

	ImGui::Text("Shoot: %d", m_Shooting);

	if (m_Context->m_pInput->MouseButtonDown(0))
	{
		m_pBAC->PlayAnimation(shoot);
		m_Shooting = 1;
	}
	else
	{
		m_Shooting = 0;
	}

	if (m_Context->m_pInput->KeyDown(DIK_W))
	{
		m_MoveDirection -= m_pTC->GetFront();
		if (m_bOnGround) m_pBAC->PlayAnimation(run);
	}
	if (m_Context->m_pInput->KeyDown(DIK_S))
	{
		m_MoveDirection += m_pTC->GetFront();
		if (m_bOnGround) m_pBAC->PlayAnimation(runBside);
	}
	if (m_Context->m_pInput->KeyDown(DIK_D))
	{
		m_MoveDirection += m_pTC->GetRight();
		if (m_bOnGround) m_pBAC->PlayAnimation(runRside);
	}
	if (m_Context->m_pInput->KeyDown(DIK_A))
	{
		m_MoveDirection -= m_pTC->GetRight();
		if (m_bOnGround) m_pBAC->PlayAnimation(runLside);
	}
	
	m_Yaw -= m_Context->m_pInput->mouseDX()*0.25f;
	glm::quat qYaw = glm::angleAxis(glm::radians(m_Yaw), glm::vec3(0, 1, 0));
	qYaw = glm::normalize(qYaw);
	glm::mat4 rotate = glm::mat4_cast(qYaw);

	vec3 pos = m_pTC->GetPosition();
	m_pTC->GetTransform() = rotate;
	m_pTC->SetPosition(pos);
	

	

}

void LocalPlayerComponent::VOnChanged(void)
{
}

const char * LocalPlayerComponent::VGetName() const
{
	return Name;
}

void LocalPlayerComponent::PhysicCollisionEvent(std::shared_ptr<const IEvent> pEvent)
{
	const EvtData_PhysCollisionStart* p = static_cast<const EvtData_PhysCollisionStart*>(pEvent.get());
	if (p->GetActorA()->GetId() == m_pOwner->GetId() && p->GetActorB()->VGetTag() =="World")
		m_bOnGround = true;
	if (p->GetActorB()->GetId() == m_pOwner->GetId() && p->GetActorA()->VGetTag() == "World")
		m_bOnGround = true;

}

void LocalPlayerComponent::PhysicPreStepEvent(std::shared_ptr<const IEvent> pEvent)
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


	if (m_bOnGround)
	{
		vec3 brakeForce = -planeVelocity * m_fBrakeForce;
		m_pRBC->ApplyImpulse(brakeForce);
		
		if (m_Context->m_pInput->KeyDown(DIK_SPACE))
		{
			m_JumpDirection = vec3(0, 1, 0);
			m_pRBC->ApplyImpulse(vec3(0, 1, 0)*m_fJumpForce);
			m_pBAC->PlayAnimation(jump, false);

		}

		if (m_MoveDirection == vec3(0) && m_JumpDirection == vec3(0)&&!m_Shooting)
		{
			m_pBAC->PlayAnimation(idle);
		}


		if (m_MoveDirection != vec3(0))
		{

			m_MoveDirection = glm::normalize(m_MoveDirection);
			m_pRBC->ApplyImpulse(m_MoveDirection *m_fMoveForce);
		}
	}

	

	if (m_Shooting)
	{
		m_pBAC->PlayAnimation(shoot);
	}
	
	uint8_t;
	
	m_bOnGround = false;
}

void LocalPlayerComponent::PhysicPostStepEvent(std::shared_ptr<const IEvent> pEvent)
{

}