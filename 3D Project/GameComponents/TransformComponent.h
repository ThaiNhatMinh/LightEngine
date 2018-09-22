#pragma once
#include "..\Interface\IComponent.h"


namespace Light
{
	class TransformComponent : public ITransformComponent
	{
	public:

		virtual bool VSerialize(IContext*pContext,const tinyxml2::XMLElement * pData);
		virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);

		virtual glm::vec3	GetFront()override;
		virtual glm::vec3	GetRight()override;
		virtual glm::vec3	GetUp()override;
		virtual glm::vec3	GetPos()override;
		virtual glm::quat  GetOrientation();

		virtual void		SetPos(glm::vec3 pos)override;
		virtual void		SetTransform(glm::vec3 pos, glm::quat quad) override;
		virtual void		SetOrientation(glm::quat ort) override;
		virtual glm::mat4	GetTransform();

	private:
		void BuildMatrix();
	private:
		glm::vec3 m_Position;
		glm::quat m_Orientation;
		glm::vec3 m_Scale;
		glm::mat4 transform;
	};
}