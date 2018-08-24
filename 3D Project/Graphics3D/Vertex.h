#pragma once

#include <glm\vec3.hpp>
#include <glm\vec2.hpp>
#include <glm\vec4.hpp>


struct DefaultVertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};


struct imguiVertex
{
	glm::vec2 pos;
	glm::vec2 uv;
	glm::vec4 color;
};

struct Weight
{
	Weight(int b, float w) { Bone = (float)b; weight = w; };
	Weight() :Bone(0), weight(0) {};
	float Bone;
	float weight;
};

struct SkeVertex
{
	glm::vec3		pos;
	glm::vec3		normal;
	glm::vec2		uv;
	Weight		weights[4];

};