#pragma once

#define TYPE_DECAL 1<<1
#define TYPE_PARTICLE 1<<2

struct Particle
{
	float CameraDistance;
	glm::vec3 Pos;
	glm::vec3 color;
	glm::vec2 size;

	float life;

	bool Update(float dt) { return 0; }

	bool operator<(const Particle& other)
	{
		return CameraDistance > other.CameraDistance;
	}
};

struct Decal : public Particle
{
	glm::vec3 Front; // normal vector in plane
};