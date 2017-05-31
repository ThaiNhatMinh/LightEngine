#include "..\pch.h"
#include "AABB.h"

AABB::AABB()
{
	Min = vec3(FLT_MAX);
	Max = vec3(FLT_MIN);
}

void AABB::Set(const vector<vec3>& v)
{
	//Max = v[0];
	//Min = v[0];
	for (GLuint i = 0; i < v.size(); i++)
	{
		if (Min.x > v[i].x) Min.x = v[i].x;
		else if (Max.x < v[i].x) Max.x = v[i].x;
		if (Min.y > v[i].y) Min.y = v[i].y;
		else if (Max.y < v[i].y) Max.y = v[i].y;
		if (Min.z > v[i].z) Min.z = v[i].z;
		else if (Max.z < v[i].z) Max.x = v[i].z;
	}
}

bool AABB::Intersect(const AABB & a)
{
	if (a.Min.x > Max.x || Min.x > a.Max.x) return false;
	if (a.Min.y > Max.y|| Min.y > a.Max.y) return false;
	if (a.Min.z > Max.z || Min.z > a.Max.z) return false;
	return true;
}
