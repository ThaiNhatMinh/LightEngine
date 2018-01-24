#pragma once

class Ray
{

public:
	Ray() = default;
	Ray(const vec3& pos, const vec3& direction) :pos(pos), direction(direction) {};

public:
	vec3 pos;
	vec3 direction;
};