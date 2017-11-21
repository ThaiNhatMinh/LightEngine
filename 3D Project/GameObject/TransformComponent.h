#pragma once
#include "pch.h"

// Every object in Scene must have this component
// TransformComponent only store transform with parent transform. Not global transform
class TransformComponent : public ActorComponent
{
private:
	mat4 m_Transform;
public:
	static const char* Name;
	virtual const char* VGetName() const { return Name; }
	TransformComponent(void)  { }
	TransformComponent(const mat4& t) { m_Transform = t; }
	virtual bool VInit(const tinyxml2::XMLElement* pData) ;
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p) ;

	// transform functions
	mat4 GetTransform(void) const { return m_Transform; }
	void SetTransform(const mat4& newTransform) { m_Transform = newTransform; }
	vec3 GetPosition(void) const { return vec3(m_Transform[3]); }
	void SetPosition(const vec3& pos) { m_Transform = glm::translate(m_Transform,pos); }

	vec3 GetFront();
	vec3 GetRight();
	vec3 GetUp();

};