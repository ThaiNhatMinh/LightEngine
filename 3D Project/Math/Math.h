#pragma once


namespace Light
{
	namespace Math
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
	}
}