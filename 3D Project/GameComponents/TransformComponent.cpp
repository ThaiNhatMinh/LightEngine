#include "pch.h"
#include "TransformComponent.h"

bool Light::TransformComponent::VSerialize(IContext*pContext,const tinyxml2::XMLElement * pData)
{
	if (!pData) return false;

	vec3 yawPitchRoll;
	
	m_Scale = glm::vec3(1, 1, 1);

	const tinyxml2::XMLElement* pPositionElement = pData->FirstChildElement("Position");
	if (pPositionElement)
	{
		float x = 0;
		float y = 0;
		float z = 0;
		x = pPositionElement->FloatAttribute("x");
		y = pPositionElement->FloatAttribute("y");
		z = pPositionElement->FloatAttribute("z");
		m_Position = vec3(x, y, z);
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
		m_Orientation = glm::toQuat(glm::orientate3(yawPitchRoll));
	}

	
	const tinyxml2::XMLElement* pScaleElement = pData->FirstChildElement("Scale");
	if (pScaleElement)
	{
		float x = 1.0;
		float y = 1.0;
		float z = 1.0;
		x = pScaleElement->FloatAttribute("x");
		y = pScaleElement->FloatAttribute("y");
		z = pScaleElement->FloatAttribute("z");
		m_Scale = vec3(x, y, z);
		
	}

	//glm::mat4 matRoll = glm::mat4(1.0f);//identity matrix; 
	//glm::mat4 matPitch = glm::mat4(1.0f);//identity matrix
	//glm::mat4 matYaw = glm::mat4(1.0f);//identity matrix

	//								   //roll, pitch and yaw are used to store our angles in our class
	//matRoll = glm::rotate(matRoll, yawPitchRoll.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//matPitch = glm::rotate(matPitch, yawPitchRoll.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//matYaw = glm::rotate(matYaw, yawPitchRoll.y, glm::vec3(0.0f, 1.0f, 0.0f));

	//
	////order matters
	//glm::mat4 rotate = matRoll * matPitch * matYaw;


	BuildMatrix();



	return true;
}

tinyxml2::XMLElement * Light::TransformComponent::VDeserialize(tinyxml2::XMLDocument * p)
{
	auto pBaseNode = p->NewElement("TransformComponent");
	auto pPosNode = p->NewElement("Position");
	pPosNode->SetAttribute("x",0);
	auto pRotateNode = p->NewElement("YawPitchRoll");


	return nullptr;
}

glm::vec3 Light::TransformComponent::GetFront()
{
	return transform[2];
}

glm::vec3 Light::TransformComponent::GetRight()
{
	return transform[0];
}

glm::vec3 Light::TransformComponent::GetUp()
{
	return transform[1];
}

glm::vec3 Light::TransformComponent::GetPos()
{
	return m_Position;
}

glm::quat Light::TransformComponent::GetOrientation()
{
	return m_Orientation;
}

glm::vec3 Light::TransformComponent::GetScale()
{
	return m_Scale;
}

void Light::TransformComponent::SetScale(glm::vec3 s)
{
	m_Scale = s;

	BuildMatrix();
}

void Light::TransformComponent::SetPos(glm::vec3 pos)
{
	m_Position = pos;
	transform[3] = glm::vec4(m_Position,1.0f);
}

void Light::TransformComponent::SetTransform(glm::vec3 pos, glm::quat orientation)
{
	m_Position = pos;
	m_Orientation = orientation;
	BuildMatrix();
}

void Light::TransformComponent::SetOrientation(glm::quat ort)
{
	m_Orientation = ort;
	BuildMatrix();
}

glm::mat4  Light::TransformComponent::GetTransform()
{
	return transform;
}

void Light::TransformComponent::BuildMatrix()
{
	glm::mat4 rotate = glm::mat4_cast(m_Orientation);
	glm::mat4 scale = glm::scale(mat4(), m_Scale);
	glm::mat4 translate = glm::translate(glm::mat4(), m_Position);

	transform = (translate*rotate*scale);
}
