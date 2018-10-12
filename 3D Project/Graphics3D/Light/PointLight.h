#pragma once

#include "..\Light.h"
#include <glm\vec3.hpp>

namespace Light
{
	namespace render
	{
		class PointLight : public LightData
		{
		public:
			glm::vec3 Ia;
			glm::vec3 Id;
			glm::vec3 Is;
			glm::vec3 Pos;
			float constant;
			float linear;
			float quadratic;
			virtual void SetupParam(LightParam*)override;

		};
	}
}