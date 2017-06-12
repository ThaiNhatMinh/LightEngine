
#pragma once

#include "..\pch.h"
// ax + by + cz = d
// Normal is (a,b,c)
// Offset is d
class Plane
{
public:
	vec3 m_Normal;
	float m_Offset;
	Plane(){};
	Plane(float a, float b, float c, float d);
	Plane(vec3 normal, float offset) :m_Normal(normal), m_Offset(offset) {}
	// tree point in counter clockwise order
	void Init(const vec3& v1, const vec3& v2, const vec3& v3);


	bool Inside(const vec3& v, float darius) const;
	bool inline Inside(const vec3& v)const {
		return (glm::dot(m_Normal, v) == m_Offset);
	};
	float inline DistanceToPlane(const vec3& v)const {
		float result = glm::dot(m_Normal, v) - m_Offset;
		return result;
	}
	//bool IntersectionLine(const vec3& v1, const vec3& v2) const;

};