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
	tinyxml2::XMLElement* pIns = pData->FirstChildElement("Frustum");
	if (pIns == nullptr) return false;


	float fov = pIns->DoubleAttribute("fov", 45.0f);
	float as = pIns->DoubleAttribute("aspect", 4.0/3.0);
	float np = pIns->DoubleAttribute("nplane", 1.0f);
	float fp = pIns->DoubleAttribute("fplane", 500.0f);
	
	if (as < 0.0f)
	{
		// [TODO] using windows subsystem to get aspect raito
	}

	m_Frustum  = Frustum(fov, as, np, fp);

	return true;
}

void CameraComponent::VPostInit(void)
{
	m_pTransform = m_pOwner->GetComponent<TransformComponent>(TransformComponent::Name);
}
/*
void CameraComponent::VUpdate(float dt)
{
	vec3 pos = m_pTransform->GetPosition();
	vec3 front = m_pTransform->GetFront();
	vec3 up = m_pTransform->GetUp();
	ViewMatrix = glm::lookAt(pos, pos + front, up);
	//ViewMatrix = glm::inverse(m_pTransform->GetTransform());

}
*/

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
	vec3 pos = m_pTransform->GetPosition();
	vec3 front = m_pTransform->GetFront();
	vec3 up = m_pTransform->GetUp();
	ViewMatrix = glm::lookAt(pos, pos + front, up);
	m_Frustum.Update(pos, front, m_pTransform->GetRight());
	return ViewMatrix;
}

mat4 CameraComponent::GetProjMatrix()
{
	return m_Frustum.GetProjMatrix();
}

mat4 CameraComponent::GetVPMatrix()
{
	return GetViewMatrix()*m_Frustum.GetProjMatrix();
}

const Frustum& CameraComponent::GetFrustum()const
{
	return m_Frustum;
}

Frustum& CameraComponent::GetFrustum()
{
	return m_Frustum;
}