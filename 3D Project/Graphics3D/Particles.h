#pragma once
#include "Renderer.h"

#define TYPE_DECAL 1<<1
#define TYPE_PARTICLE 1<<2
namespace Light
{
	namespace render
	{
		struct BaseParticle
		{
			float CameraDistance;
			bool operator<(const BaseParticle& other)
			{
				return CameraDistance > other.CameraDistance;
			}
		};
		struct Particle:public BaseParticle
		{
			Texture* Tex;
			
			glm::vec3 Pos;
			glm::vec3 color;
			glm::vec2 size;

			float life; // life time 

			
		};

		struct Decal : public Particle
		{
			glm::vec3 normal; // normal vector in plane
		};


		struct Billboard : public Particle
		{
			glm::vec3 front;
		};

	}
}