#pragma once
#include "pch.h"


class Material
{
public:

	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	vec3 exp;
	Material() :Ka(1.0f), Kd(1.0f), Ks(1.0f), exp(128)
	{

	}
};