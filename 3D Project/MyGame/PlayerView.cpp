#include <pch.h>
#include "PlayerView.h"

PlayerView::PlayerView(ActorId id) :Actor(id)
{
}

bool PlayerView::Init(const tinyxml2::XMLElement * pData)
{
	bool r = Actor::Init(pData);

	/*const tinyxml2::XMLElement * pNode = pData->FirstChildElement("PVTransform");
	
	const tinyxml2::XMLElement* pPositionElement = pNode->FirstChildElement("Position");
	vec3 position, yawPitchRoll;
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

	const tinyxml2::XMLElement* pOrientationElement = pNode->FirstChildElement("YawPitchRoll");
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
	translation = glm::translate(translation, position);

	mat4 rotation = glm::eulerAngleYXZ(yawPitchRoll.y, yawPitchRoll.x, yawPitchRoll.z);

	m_PVTransform = translation*rotation;

	pNode = pNode->FirstChildElement("FOV");

	m_fFOV = pNode->DoubleAttribute("fov", 45.0);*/

	return 1;
}

void PlayerView::PostInit(void)
{
	Actor::PostInit();

}

HRESULT PlayerView::VRender(Scene * pScene)
{
	vec3 pos = VGetGlobalTransform()[3];
	ImGui::Text("Pos: %f %f %f", pos.x, pos.y, pos.z);
	GetComponent<MeshRenderComponent>("MeshRenderComponent")->Render(pScene);
	return S_OK;
}
