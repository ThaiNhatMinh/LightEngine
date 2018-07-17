#pragma once

#include <glm/vec3.hpp>
// Axis-Aligned Bounding Boxes
namespace Light
{
	namespace math
	{
		class AABB
		{
		public:
			glm::vec3 Min;
			glm::vec3 Max;
		public:
			AABB();
			~AABB() {};


			void Set(const std::vector<glm::vec3>&);
			void Test(const glm::vec3& v);
			void SetMin(const glm::vec3& min) {
				Min = min;
			};
			void SetMax(const glm::vec3& max) {
				Max = max;
			};
			bool Intersect(const AABB& a);
			void Insert(const glm::vec3& v);
			void GenPoint(glm::vec3 a[8]);
			void Translate(const glm::vec3& v)
			{
				Min += v;
				Max += v;
			}
		};
	}
}