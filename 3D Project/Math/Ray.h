#pragma once
#include <glm.hpp>

namespace Light
{
	namespace math
	{
		class Ray
		{

		public:
			Ray() = default;
			Ray(const glm::vec3& pos, const glm::vec3& direction) :pos(pos), direction(direction) {};

		public:
			glm::vec3 pos;
			glm::vec3 direction;
		};
	}
}