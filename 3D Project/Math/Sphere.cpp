#include "..\pch.h"

BVSphere::BVSphere()
{
	m_Pos = vec3(0.0f);
	m_fRadius = 1.0f;
}

void BVSphere::Set(const vector<vec3>& v)
{
	vec3 Max(v[0]), Min(v[0]);
	for (GLuint i = 1; i < v.size(); i++)
	{
		if (Min.x > v[i].x) Min.x = v[i].x;
		else if (Max.x < v[i].x) Max.x = v[i].x;
		if (Min.y > v[i].y) Min.y = v[i].y;
		else if (Max.y < v[i].y) Max.y= v[i].y;
		if (Min.z > v[i].z) Min.z = v[i].z;
		else if (Max.z < v[i].z) Max.x = v[i].z;
	}

	m_Pos = (Min + Max) / 2.0f;
	m_fRadius = Math::DistanceSquare(v[0], m_Pos);
	for (GLuint i = 1; i < v.size(); i++)
	{
		float distance = Math::DistanceSquare(v[i], m_Pos);
		if (distance < m_fRadius) m_fRadius = distance;
	}

	m_fRadius = sqrtf(m_fRadius);
}

bool BVSphere::Intersect(const BVSphere & s)
{
	vec3 vs = m_Pos - s.m_Pos;
	float t = m_fRadius - s.m_fRadius;
	return (Math::Dot(vs, vs) <= t*t);
}

bool BVSphere::Intersect(const vec3 & pos, const vec3 & direction)
{
	vec3 w = m_Pos - pos;
	float wsq = Math::Dot(w, w);
	float proj = Math::Dot(w, direction);
	float rsq = m_fRadius*m_fRadius;

	if (proj<0.0f && wsq > rsq) return false;

	float vsq = Math::Dot(direction, direction);

	return (vsq*wsq - proj*proj <= vsq*rsq);
}

float BVSphere::Classify(const Plane & p)
{
	float t = p.DistanceToPlane(m_Pos);
	if (t > m_fRadius) return t - m_fRadius;
	else if (t < -m_fRadius) return t + m_fRadius;
	else return 0;
}
