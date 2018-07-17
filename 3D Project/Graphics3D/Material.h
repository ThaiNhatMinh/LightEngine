#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
class Material
{
public:
	std::string Name;
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	glm::vec3 exp;
	Material() :Ka(1.0f), Kd(1.0f), Ks(1.0f), exp(1,1,1)
	{

	}
};