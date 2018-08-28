#pragma once
#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>
#include "..\Math\Frustum.h"
namespace Light
{
	namespace render
	{
		class math::Frustum;
		class ICamera
		{

		public:
			virtual ~ICamera() {};
			virtual const glm::mat4& GetViewMatrix() = 0;
			virtual const glm::mat4& GetProjMatrix() = 0;
			virtual math::Frustum*	GetFrustum() = 0;
		};
	}
}