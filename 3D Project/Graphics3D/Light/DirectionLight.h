#pragma once
#include "..\Light.h"
#include <glm\vec3.hpp>

namespace Light
{
	namespace render
	{
		class DirectionLight : public LightData
		{
		public:
			glm::vec3 Ia;
			glm::vec3 Id;
			glm::vec3 Is;
			glm::vec3 direction;
			virtual void SetupParam(LightParam*)override;

		};
	}
}