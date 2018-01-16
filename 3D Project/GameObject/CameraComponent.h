#pragma once

#include <pch.h>

class CameraComponent : public ActorComponent
{
private:
	vec3 WorldUp;
	mat4 ViewMatrix;
	TransformComponent* m_pTransform;
	Frustum			m_Frustum;
	vec3			m_Right;
	vec3			m_Up;
	vec3			m_Front;
	vec3			m_Pos;
public:
	static vector<CameraComponent*> m_CameraList;
	static const char* Name;
	CameraComponent();
	~CameraComponent();
	virtual bool VInit(const tinyxml2::XMLElement* pData) ;
	virtual void VPostInit(void);
	virtual void VUpdate(float dt);
	//virtual void VUpdate(float dt);
	virtual const char *VGetName() const;
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p);

	const mat4& GetViewMatrix();
	const mat4& GetProjMatrix();
	const mat4& GetVPMatrix();
	const vec3& GetUp();
	const vec3& GetRight();
	const vec3& GetFront();
	const vec3& GetPos();
	const Frustum& GetFrustum()const;
	Frustum& GetFrustum();
	
};