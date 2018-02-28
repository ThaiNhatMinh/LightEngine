#include "pch.h"

const char* CameraComponent::Name = "CameraComponent";
CameraComponent::CameraComponent()
{
}

CameraComponent::~CameraComponent()
{
}

bool CameraComponent::VInit(const tinyxml2::XMLElement* pData)
{
	if (!pData) return false;
	const tinyxml2::XMLElement* pIns = pData->FirstChildElement("Frustum");
	if (pIns == nullptr) return false;


	float fov = pIns->DoubleAttribute("fov", 45.0f);
	float as = pIns->DoubleAttribute("aspect", 4.0/3.0);
	float np = pIns->DoubleAttribute("nplane", 1.0f);
	float fp = pIns->DoubleAttribute("fplane", 500.0f);
	
	if (as < 0.0f)
	{
		vec2 size = m_Context->GetSystem<Windows>()->GetWindowSize();
		as = size[0]/size[1];
	}

	WorldUp = vec3(0, 1, 0);
	m_Frustum  = Frustum(fov, as, np, fp);

	return true;
}

const char * CameraComponent::VGetName() const
{
	return Name;
}

tinyxml2::XMLElement * CameraComponent::VGenerateXml(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

void CameraComponent::UpdateAngle(float yaw, float pitch)
{
	vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));



	m_Front = glm::normalize(front);
	m_Right = glm::normalize(glm::cross(m_Front, WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	ViewMatrix = glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
	m_Frustum.Update(m_Pos, m_Front, m_Right);

}


const mat4& CameraComponent::GetViewMatrix()
{
	/*vec3 pos = m_pTransform->GetPosition();
	vec3 front = m_pTransform->GetFront();
	vec3 up = m_pTransform->GetUp();
	ViewMatrix = glm::lookAt(pos, pos + front, up);
	m_Frustum.Update(pos, front, m_pTransform->GetRight());*/
	return ViewMatrix;
}

void CameraComponent::VUpdate(float dt)
{
	mat4 tt = m_pOwner->VGetGlobalTransform();

	m_Pos = tt[3];
	m_Right = tt[0];
	m_Up = tt[1];
	m_Front = tt[2];

	ViewMatrix = glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);

	m_Frustum.Update(m_Pos, m_Front, m_Right);

	VPMatrix = m_Frustum.GetProjMatrix()*ViewMatrix;
	
}
void CameraComponent::VPostInit(void)
{
	Camera::SetCurrentCamera(this);
}
const mat4& CameraComponent::GetProjMatrix()
{
	return m_Frustum.GetProjMatrix();
}

const mat4& CameraComponent::GetVPMatrix()
{
	return VPMatrix;
}

const vec3 & CameraComponent::GetUp() { return m_Up; }

const vec3 & CameraComponent::GetRight() { return m_Right; }

const vec3 & CameraComponent::GetFront() { return m_Front; }

const vec3 & CameraComponent::GetPosition() {
	return m_Pos;
}

Frustum* CameraComponent::GetFrustum()
{
	return &m_Frustum;
}