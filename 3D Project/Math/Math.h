#pragma once

#include "AABB.h"
namespace Light
{
	namespace math
	{
		template<typename T>
		std::vector<T> CopySubMatrix(std::vector<T> array, int pos[2], int subsize)
		{
			std::vector<T> result;
			int size = sqrt(array.size());
			result.resize(subsize*subsize);
			int offset = 0;
			for (int i = 0; i < subsize; i++)
			{
				offset = (pos[0] + i)*size + pos[1];
				std::memcpy(&result[i*subsize], &array[offset], sizeof(T) * subsize);

			}
			return result;
		}

		static AABB TrasformAABB(AABB box,glm::mat4 trasform)
		{
			glm::vec3 v[8];
			box.GenPoint(v);
			AABB newbox;

			for (int i = 0; i < 8; i++)
			{
				v[i] = trasform * glm::vec4(v[i],1.0f);
				newbox.Test(v[i]);
			}
			return newbox;
		}

		std::vector<DefaultVertex> GenerateVertexData(resources::HeightMapData * hm, int size, int w, int h, float hscale, int sub);
		std::vector<unsigned int> GenerateIndicesData(resources::HeightMapData * hm,int numSub);
		std::vector<unsigned int> GenerateIndicesData(resources::HeightMapData * hm);
		
	}
}