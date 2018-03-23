#include <pch.h>

Plane::Plane(float a, float b, float c, float d)
{
	m_Normal.x = a;
	m_Normal.y = b;
	m_Normal.z = c;
	m_Offset = d;
}

void Plane::Init(const vec3& v1, const vec3& v2, const vec3& v3)
{
	vec3 e1 = v1 - v2;
	vec3 e2 = v3 - v2;
	m_Normal = glm::normalize(glm::cross(e1,e2));
	m_Offset = glm::dot(m_Normal, v1);
}



bool Plane::Inside(const vec3& v, float darius)const
{
	float d = glm::dot(m_Normal, v) + m_Offset;
	return (d>=-darius);
}