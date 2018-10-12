#pragma once
#include "..\Interface\IScript.h"
#include "..\Interface\IComponent.h"
#include "..\Interface\IInput.h"

namespace Light
{
	class CameraControl : public IScript
	{
		ITransformComponent* m_pTransform;
		IInput* m_pInput;
		glm::vec3 m_Front, m_Right, m_Position;
		float m_Pitch, m_Yaw;
		float MouseSensitivity;
		float m_Speed;
	public:
		CameraControl(IContext* pContext, IActor* owner);
		virtual bool VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
		virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);
		virtual void PostInit();
		virtual void Update(float dt);
	};
}