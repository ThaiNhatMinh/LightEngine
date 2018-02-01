#include "pch.h"
#include "LogicComponent.h"

const char* LogicComponent::Name = "LogicComponent";
LogicComponent::LogicComponent()
{
	MouseSensitivity = 0.25;
	m_Speed = 400.0f;
	m_Pitch = 0;
	m_Yaw = -90;
}


LogicComponent::~LogicComponent()
{
	m_Context->m_pEventManager->VRemoveListener(MakeDelegate(this, &LogicComponent::PhysicPreStepEvent), EvtData_PhysPreStep::sk_EventType);
	m_Context->m_pEventManager->VRemoveListener(MakeDelegate(this, &LogicComponent::PhysicPostStepEvent), EvtData_PhysPostStep::sk_EventType);
}

tinyxml2::XMLElement * LogicComponent::VGenerateXml(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

const char * LogicComponent::VGetName() const
{
	return Name;
}

void LogicComponent::VPostInit(void)
{
	// register event
	m_Context->m_pEventManager->VAddListener(MakeDelegate(this, &LogicComponent::PhysicPreStepEvent), EvtData_PhysPreStep::sk_EventType);
	m_Context->m_pEventManager->VAddListener(MakeDelegate(this, &LogicComponent::PhysicPostStepEvent), EvtData_PhysPostStep::sk_EventType);

	m_TF = m_pOwner->GetTransform();
	m_Position = m_TF->GetPosition();
}

void LogicComponent::VUpdate(float deltaTIme)
{
	
	// Camera controls
	m_Front = m_TF->GetFront();
	m_Right = m_TF->GetRight();

	if (m_Context->m_pInput->KeyDown(DIK_W)) m_Position += m_Front*m_Speed*deltaTIme;
	if (m_Context->m_pInput->KeyDown(DIK_S)) m_Position -= m_Front*m_Speed*deltaTIme;
	if (m_Context->m_pInput->KeyDown(DIK_D)) m_Position -= m_Right*m_Speed*deltaTIme;
	if (m_Context->m_pInput->KeyDown(DIK_A)) m_Position += m_Right*m_Speed*deltaTIme;

	m_Pitch -= m_Context->m_pInput->mouseDY()*MouseSensitivity;
	m_Yaw -= m_Context->m_pInput->mouseDX()*MouseSensitivity;
	if (m_Pitch > 89.0f) m_Pitch = 89.0f;
	if (m_Pitch < -89.0f) m_Pitch = -89.0f;

	//vec3 front;
	//front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	//front.y = sin(glm::radians(m_Pitch));
	//front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	//m_TF->SetPosition(m_Position);

	//FPS camera:  RotationX(pitch) * RotationY(yaw)
	glm::quat qPitch = glm::angleAxis(glm::radians(m_Pitch), glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(glm::radians(m_Yaw), glm::vec3(0, 1, 0));
	glm::quat qRoll = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));

	//For a FPS camera we can omit roll
	glm::quat orientation = qYaw*qPitch;
	orientation = glm::normalize(orientation);
	glm::mat4 rotate = glm::mat4_cast(orientation);

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, m_Position);
	m_TF->SetTransform(translate*rotate);
	
}

void LogicComponent::FixedUpdate(float timeStep)
{
}

void LogicComponent::FixedPostUpdate(float timeStep)
{
}

void LogicComponent::PhysicPreStepEvent(std::shared_ptr<IEvent> pEvent)
{
	const EvtData_PhysPreStep *p = static_cast<const EvtData_PhysPreStep*>(pEvent.get());
	FixedUpdate(p->GetTimeStep());

}

void LogicComponent::PhysicPostStepEvent(std::shared_ptr<IEvent> pEvent)
{
	const EvtData_PhysPostStep *p = static_cast<const EvtData_PhysPostStep*>(pEvent.get());
	FixedPostUpdate(p->GetTimeStep());
}
