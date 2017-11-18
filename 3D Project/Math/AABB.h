#pragma once
#include "pch.h"


// Axis-Aligned Bounding Boxes

class AABB
{
public:
	vec3 Min;
	vec3 Max;
public:
	AABB();
	~AABB() {};


	void Set(const vector<vec3>&);
	void SetMin(const vec3& min) {
		Min = min;
	};
	void SetMax(const vec3& max) {
		Max = max;
	};
	bool Intersect(const AABB& a);

	void Translate(const vec3& v)
	{
		Min += v;
		Max += v;
	}
};