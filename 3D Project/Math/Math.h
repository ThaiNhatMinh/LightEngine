#pragma once


bool operator<(const vec3& a, const vec3& b)
{
	return a.x < b.x && a.y < b.y && a.z < b.z;
}