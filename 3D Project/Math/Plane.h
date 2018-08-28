
#pragma once

// ax + by + cz = d
// Normal is (a,b,c)
// Offset is d
#include <glm\vec3.hpp>
#include <glm\geometric.hpp>

namespace Light
{
	namespace math
	{
		class Plane
		{
		public:
			glm::vec3 m_Normal;
			float m_Offset;
			Plane() {};
			Plane(float a, float b, float c, float d);
			Plane(glm::vec3 normal, float offset) :m_Normal(normal), m_Offset(offset) {}
			// tree point in counter clockwise order
			void Init(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);


			bool Inside(const glm::vec3& v, float darius) const;
			bool inline Inside(const glm::vec3& v)const {
				return (glm::dot(m_Normal, v) == m_Offset);
			};
			float inline DistanceToPlane(const glm::vec3& v)const {
				float result = glm::dot(m_Normal, v) - m_Offset;
				return result;
			}
			//bool IntersectionLine(const glm::vec3& v1, const glm::vec3& v2) const;

		};
	}
}