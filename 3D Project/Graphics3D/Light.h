#pragma once
#include <glm\vec3.hpp>


namespace Light
{
	namespace render
	{
		class DirectionLight
		{
		public:
			glm::vec3 La;
			glm::vec3 Ld;
			glm::vec3 Ls;
			glm::vec3 direction;
		};
	}
}