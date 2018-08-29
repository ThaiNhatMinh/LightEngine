#include "pch.h"
#include "CameraControl.h"
#include "..\Interface\IActor.h"

Light::CameraControl::CameraControl(IContext * pContext, IActor * owner)
{
	m_pInput = pContext->GetSystem<IInput>();
	m_pTransform = owner->GetComponent<ITransformComponent>();

	m_Position = m_pTransform->GetPos();

	MouseSensitivity = 0.25;
	m_Speed = 100.0f;
	m_Pitch = 0;
	m_Yaw = -90;

}

bool Light::CameraControl::VSerialize(IContext * pContext, const tinyxml2::XMLElement * pData)
{
	auto pSpeedNode = pData->FirstChildElement("Speed");
	m_Speed = atof(pSpeedNode->GetText());

	auto pMouseSen = pData->FirstChildElement("MouseSensitivity");
	MouseSensitivity = atof(pMouseSen->GetText());

	return true;
}

tinyxml2::XMLElement * Light::CameraControl::VDeserialize(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

void Light::CameraControl::PostInit()
{
}

void Light::CameraControl::Update(float deltaTIme)
{
	m_Front = m_pTransform->GetFront();
	m_Right = m_pTransform->GetRight();

	if (m_pInput->VOnKey(Key::W)) m_Position += m_Front * m_Speed*deltaTIme;
	if (m_pInput->VOnKey(Key::S)) m_Position -= m_Front * m_Speed*deltaTIme;
	if (m_pInput->VOnKey(Key::D)) m_Position -= m_Right * m_Speed*deltaTIme;
	if (m_pInput->VOnKey(Key::A)) m_Position += m_Right * m_Speed*deltaTIme;

	int dx, dy;
	
	m_pInput->VMouseDeltaPos(dx, dy);

	m_Pitch -= dy*MouseSensitivity;
	m_Yaw -= dx*MouseSensitivity;

	if (m_Pitch > 89.0f) m_Pitch = 89.0f;
	if (m_Pitch < -89.0f) m_Pitch = -89.0f;

	glm::quat qPitch = glm::angleAxis(glm::radians(m_Pitch), glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(glm::radians(m_Yaw), glm::vec3(0, 1, 0));
	glm::quat qRoll = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));

	//For a FPS camera we can omit roll
	glm::quat orientation = qYaw * qPitch;
	orientation = glm::normalize(orientation);
	glm::mat4 rotate = glm::mat4_cast(orientation);

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, m_Position);
	m_pTransform->transform = (translate*rotate);
}
