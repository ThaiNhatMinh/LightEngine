#pragma once

#include "..\Interface\IComponent.h"
#include "..\Math\Frustum.h"
namespace Light
{
	class CameraComponent : public ICameraComponent
	{
	private:
		vec3 WorldUp;
		mat4 ViewMatrix;
		mat4 VPMatrix;
		math::Frustum			m_Frustum;
		vec3			m_Right;
		vec3			m_Up;
		vec3			m_Front;
		vec3			m_Pos;
	private:
	
	public:

		CameraComponent();
		~CameraComponent();
		virtual bool VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
		virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);

		virtual const mat4& GetViewMatrix() override;
		virtual const mat4& GetProjMatrix() override;
		virtual const mat4& GetVPMatrix();
		virtual const vec3& GetPosition();
		virtual math::Frustum*	GetFrustum() override;
		virtual const vec3& GetUp();
		virtual const vec3& GetRight();
		virtual const vec3& GetFront();


		void Update();

	};

}