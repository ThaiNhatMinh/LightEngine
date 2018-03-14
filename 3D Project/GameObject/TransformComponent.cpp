#include "pch.h"

const char* TransformComponent::Name = "TransformComponent";
TransformComponent::TransformComponent(void)
{
	//m_Transform = glm::scale(mat4(), vec3(1, 1, -1));
}
bool TransformComponent::VInit(const tinyxml2::XMLElement* pData)
{
	if (!pData) return false;

	vec3 yawPitchRoll;
	vec3 position;
	vec3 Scale;
	const tinyxml2::XMLElement* pPositionElement = pData->FirstChildElement("Position");
	if (pPositionElement)
	{
		float x = 0;
		float y = 0;
		float z = 0;
		x = pPositionElement->FloatAttribute("x");
		y = pPositionElement->FloatAttribute("y");
		z = pPositionElement->FloatAttribute("z");
		position = vec3(x, y, z);
	}

	const tinyxml2::XMLElement* pOrientationElement = pData->FirstChildElement("YawPitchRoll");
	if (pOrientationElement)
	{
		float yaw = 0;
		float pitch = 0;
		float roll = 0;
		pitch = pOrientationElement->FloatAttribute("x");
		yaw = pOrientationElement->FloatAttribute("y");
		roll = pOrientationElement->FloatAttribute("z");
		yawPitchRoll = vec3(glm::radians(pitch), glm::radians(yaw), glm::radians(roll));
	}


	
	glm::mat4 matRoll = glm::mat4(1.0f);//identity matrix; 
	glm::mat4 matPitch = glm::mat4(1.0f);//identity matrix
	glm::mat4 matYaw = glm::mat4(1.0f);//identity matrix

									   //roll, pitch and yaw are used to store our angles in our class
	matRoll = glm::rotate(matRoll, yawPitchRoll.z, glm::vec3(0.0f, 0.0f, 1.0f));
	matPitch = glm::rotate(matPitch, yawPitchRoll.x, glm::vec3(1.0f, 0.0f, 0.0f));
	matYaw = glm::rotate(matYaw, yawPitchRoll.y, glm::vec3(0.0f, 1.0f, 0.0f));

	//order matters
	glm::mat4 rotate = matRoll * matPitch * matYaw;

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, position);
	
	m_Transform = translate*rotate;

	const tinyxml2::XMLElement* pScaleElement = pData->FirstChildElement("Scale");
	if (pScaleElement)
	{
		float x = 0;
		float y = 0;
		float z = 0;
		x = pOrientationElement->FloatAttribute("x");
		y = pOrientationElement->FloatAttribute("y");
		z = pOrientationElement->FloatAttribute("z");

		m_Transform = glm::scale(m_Transform, vec3(x, y, z));
	}

	return true;
}

void TransformComponent::VPostInit()
{
	ColliderComponent* p = m_pOwner->GetComponent<ColliderComponent>(ColliderComponent::Name);
	if (p == nullptr)
	{
		m_Type = SHAPE_NONE;
		return;
	}


	m_Type = p->GetType();
}

tinyxml2::XMLElement * TransformComponent::VGenerateXml(tinyxml2::XMLDocument*p)
{
	tinyxml2::XMLElement* pBaseElement = p->NewElement(VGetName());
	
	// initial transform -> position
	tinyxml2::XMLElement* pPosition = p->NewElement("Position");
	vec3 pos = vec3(m_Transform[3]);
	pPosition->SetAttribute("x", ToStr(pos.x).c_str());
	pPosition->SetAttribute("y", ToStr(pos.y).c_str());
	pPosition->SetAttribute("z", ToStr(pos.z).c_str());
	pBaseElement->LinkEndChild(pPosition);

	// initial transform -> LookAt
	tinyxml2::XMLElement* pDirection = p->NewElement("YawPitchRoll");
	
	quat rotation = glm::quat_cast(m_Transform);
	vec3 orient = glm::eulerAngles(rotation);
	
	pDirection->SetAttribute("x", ToStr(orient.x).c_str());
	pDirection->SetAttribute("y", ToStr(orient.y).c_str());
	pDirection->SetAttribute("z", ToStr(orient.z).c_str());
	pBaseElement->LinkEndChild(pDirection);

	/***
	// This is not supported yet
	// initial transform -> position
	TiXmlElement* pScale = GCC_NEW TiXmlElement("Scale");
	pPosition->SetAttribute("x", ToStr(m_scale.x).c_str());
	pPosition->SetAttribute("y", ToStr(m_scale.y).c_str());
	pPosition->SetAttribute("z", ToStr(m_scale.z).c_str());
	pBaseElement->LinkEndChild(pScale);
	**/

	return pBaseElement;
}

void TransformComponent::SetTransform(const mat4 & newTransform)
{
	if (m_Type != SHAPE_CHARACTER) m_Transform = newTransform;
	else
	{
		m_Transform[3] =newTransform[3];
	}
}

vec3 TransformComponent::GetFront()
{
	return vec3(m_Transform[2]);
}

vec3 TransformComponent::GetRight()
{
	return vec3(m_Transform[0]);
}

vec3 TransformComponent::GetUp()
{
	return vec3(m_Transform[1]);
}
