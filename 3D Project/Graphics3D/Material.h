#pragma once
#include <pch.h>


class Material
{
public:
	std::string Name;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	vec3 exp;
	Material() :Ka(0.6f, 0.6f, 0.6f), Kd(0.9f, 0.9f, 0.9f), Ks(0.0f, 0.0f, 0.0f), exp(1,1,1)
	{

	}
};