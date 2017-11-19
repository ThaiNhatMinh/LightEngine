#include "pch.h"

AABB::AABB():Min(FLT_MAX),Max(FLT_MIN)
{
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

void AABB::Insert(const vec3 & v)
{
	if (v.x < Min.x) Min.x = v.x;
	if (v.y < Min.y) Min.y = v.y;
	if (v.z < Min.z) Min.z = v.z;
	if (v.x > Max.x) Max.x = v.x;
	if (v.y > Max.x) Max.y = v.y;
	if (v.z > Max.x) Max.z = v.z;
}

void AABB::GenPoint(vec3 a[8])
{
	a[0] = Min;
	a[1] = vec3(Max.x, Min.y, Min.z);
	a[2] = vec3(Max.x, Min.y, Max.z);
	a[3] = vec3(Min.x, Min.y, Max.z);

	a[4] = vec3(Min.x, Max.y, Min.z);
	a[5] = vec3(Max.x, Max.y, Min.z);
	a[6] = vec3(Max.x, Max.y, Max.z);
	a[7] = vec3(Min.x, Max.y, Max.z);
}
