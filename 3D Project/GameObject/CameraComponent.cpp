#include "pch.h"

const char* CameraComponent::Name = "CameraComponent";
vector<CameraComponent*> CameraComponent::m_CameraList;
CameraComponent::CameraComponent()
{
	m_CameraList.push_back(this);
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
		vec2 size = m_Context->m_pWindows->GetWindowSize();
		as = size[0]/size[1];
	}

	m_Frustum  = Frustum(fov, as, np, fp);

	return true;
}

void CameraComponent::VPostInit(void)
{
	m_pTransform = m_pOwner->GetTransform();
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
	vec3 pos = tt[3];
	vec3 front = tt[2];
	vec3 up = tt[1];

	ViewMatrix = glm::lookAt(pos, pos + front, up);
	m_Frustum.Update(pos, front, tt[0]);

	ImGui::Text("Pos: %f %f %f", pos.x, pos.y, pos.z);
}
const mat4& CameraComponent::GetProjMatrix()
{
	return m_Frustum.GetProjMatrix();
}

const mat4& CameraComponent::GetVPMatrix()
{
	return m_Frustum.GetProjMatrix()*ViewMatrix;
}

const Frustum& CameraComponent::GetFrustum()const
{
	return m_Frustum;
}

Frustum& CameraComponent::GetFrustum()
{
	return m_Frustum;
}

vec3 CameraComponent::GetPosition()
{
	return m_pTransform->GetPosition();
}
