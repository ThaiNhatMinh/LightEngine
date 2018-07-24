#include "pch.h"
#include "TransformComponent.h"

bool Light::TransformComponent::VSerialize(const tinyxml2::XMLElement * pData)
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

	mat4 scale;
	const tinyxml2::XMLElement* pScaleElement = pData->FirstChildElement("Scale");
	if (pScaleElement)
	{
		float x = 0;
		float y = 0;
		float z = 0;
		x = pScaleElement->FloatAttribute("x");
		y = pScaleElement->FloatAttribute("y");
		z = pScaleElement->FloatAttribute("z");

		scale = glm::scale(mat4(), vec3(x, y, z));
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

	transform = translate * rotate*scale;



	return true;
}
