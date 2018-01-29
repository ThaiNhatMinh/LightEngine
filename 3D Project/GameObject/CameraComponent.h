#pragma once

#include <pch.h>

class CameraComponent : public ActorComponent,public Camera
{
private:
	vec3 WorldUp;
	mat4 ViewMatrix;
	mat4 VPMatrix;
	Frustum			m_Frustum;
	vec3			m_Right;
	vec3			m_Up;
	vec3			m_Front;
	vec3			m_Pos;
public:
	static const char* Name;
	CameraComponent();
	~CameraComponent();
	virtual bool VInit(const tinyxml2::XMLElement* pData) ;
	virtual void VUpdate(float dt);
	virtual void VPostInit(void);
	virtual const char *VGetName() const;
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p);
	void UpdateAngle(float yaw, float pitch);
	
	virtual const mat4& GetViewMatrix() override;
	virtual const mat4& GetProjMatrix() override;
	virtual const mat4& GetVPMatrix() override;
	virtual const vec3& GetPosition() override;
	virtual Frustum*	GetFrustum() override;
	virtual const vec3& GetUp() override;
	virtual const vec3& GetRight() override;
	virtual const vec3& GetFront() override;
	
};