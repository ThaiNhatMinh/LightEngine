#include "pch.h"

const char* CameraComponent::Name = "CameraComponent";
CameraComponent::CameraComponent()
{
}

CameraComponent::~CameraComponent()
{
}

bool CameraComponent::VInit(tinyxml2::XMLElement * pData)
{
	return true;
}

void CameraComponent::VPostInit(void)
{
	m_pTransform = m_pOwner->GetComponent<TransformComponent>(TransformComponent::Name);
}

void CameraComponent::VUpdate(float dt)
{
	vec3 pos = m_pTransform->GetPosition();
	vec3 front = m_pTransform->GetFront();
	vec3 up = m_pTransform->GetUp();
	ViewMatrix = glm::lookAt(pos, pos + front, up);
	//ViewMatrix = glm::inverse(m_pTransform->GetTransform());

}

const char * CameraComponent::VGetName() const
{
	return Name;
}

tinyxml2::XMLElement * CameraComponent::VGenerateXml(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

mat4 CameraComponent::GetViewMatrix()
{
	return ViewMatrix;
}
