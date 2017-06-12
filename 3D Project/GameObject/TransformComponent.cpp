#include "..\pch.h"

const char* TransformComponent::Name = "TransformComponent";
bool TransformComponent::VInit(tinyxml2::XMLElement * pData)
{
	if (!pData) return false;

	vec3 yawPitchRoll;
	vec3 position;
	vec3 Scale;
	tinyxml2::XMLElement* pPositionElement = pData->FirstChildElement("Position");
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

	tinyxml2::XMLElement* pOrientationElement = pData->FirstChildElement("YawPitchRoll");
	if (pOrientationElement)
	{
		float yaw = 0;
		float pitch = 0;
		float roll = 0;
		pitch = pOrientationElement->FloatAttribute("x");
		yaw = pOrientationElement->FloatAttribute("y");
		roll = pOrientationElement->FloatAttribute("z");
		yawPitchRoll = vec3(pitch, yaw, roll);
	}


	mat4 translation;
	translation= glm::translate(translation,position);

	mat4 rotation = glm::eulerAngleYXZ(yawPitchRoll.y, yawPitchRoll.x, yawPitchRoll.z);
	
	m_Transform = translation*rotation;

	return true;
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