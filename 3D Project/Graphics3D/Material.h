#pragma once
#include <string>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Renderer.h"
#include "..\Utilities\Utility.h"
#include "..\Interface\IActor.h"
#include "..\typedef.h"
namespace Light
{
	namespace render
	{
		class Material: public util::Serialization
		{
		public:
			
			std::shared_ptr<Pipeline> m_Pipeline;
			std::string Name;
			glm::vec3 Ka;
			glm::vec3 Kd;
			glm::vec3 Ks;
			glm::vec3 exp;
			Material() :Ka(1.0f), Kd(1.0f), Ks(1.0f), exp(1, 1, 1)
			{

			}
			virtual void Apply(IActor* pActor) = 0;
			virtual void ApplyMatrix(float* model, float* mvp) = 0;
			virtual MaterialType GetType() = 0;
		};
	}
}

