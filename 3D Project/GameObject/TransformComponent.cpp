#include "..\pch.h"

const char* TransformComponent::Name = "TransformComponent";
bool TransformComponent::VInit(tinyxml2::XMLElement * pData)
{
	if (!pData) return false;
	EulerAngle yawPitchRoll;

	vec3 position;

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
		yawPitchRoll = EulerAngle(pitch, yaw, roll);
	}

	mat4 translation;
	translation.Translate(position);

	mat4 rotation;
	yawPitchRoll.ToMatrixXYZ(rotation);
	
	m_Transform = translation*rotation;
	//rotation.BuildYawPitchRoll((float)DEGREES_TO_RADIANS(yawPitchRoll.x), (float)DEGREES_TO_RADIANS(yawPitchRoll.y), (float)DEGREES_TO_RADIANS(yawPitchRoll.z));
	return true;
}

tinyxml2::XMLElement * TransformComponent::VGenerateXml(tinyxml2::XMLDocument*p)
{
	tinyxml2::XMLElement* pBaseElement = p->NewElement(VGetName());
	
	// initial transform -> position
	tinyxml2::XMLElement* pPosition = p->NewElement("Position");
	vec3 pos(m_Transform.GetTranslate());
	pPosition->SetAttribute("x", ToStr(pos.x).c_str());
	pPosition->SetAttribute("y", ToStr(pos.y).c_str());
	pPosition->SetAttribute("z", ToStr(pos.z).c_str());
	pBaseElement->LinkEndChild(pPosition);

	// initial transform -> LookAt
	tinyxml2::XMLElement* pDirection = p->NewElement("YawPitchRoll");
	EulerAngle orient;
	m_Transform.ToEulerAngle(orient);
	pDirection->SetAttribute("x", ToStr(orient.pitch).c_str());
	pDirection->SetAttribute("y", ToStr(orient.yaw).c_str());
	pDirection->SetAttribute("z", ToStr(orient.roll).c_str());
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
