#include "pch.h"
#include "Math.h"

std::vector<DefaultVertex> Light::math::GenerateVertexData(resources::HeightMap * hm, int stepsize, int width, int height, float hscale, int sub)
{
	vec2 size = vec2((width - 1)*stepsize, (height - 1)*stepsize);
	vec2 size2 = vec2((width - 0)*stepsize, (height - 0)*stepsize);
	//Mesh* p = DEBUG_NEW Mesh;

	std::vector<DefaultVertex> vertex;


	int x = -size[0] / 2, y = 0, z = -size[1] / 2;
	float t = (hm->minH + hm->maxH) / 2.0f;

	auto heightF = [hm, width, height, t, hscale](int x, int z) {
		if (x < 0) x = 0;
		if (z < 0) z = 0;
		if (x >= width) x = width;
		if (z >= height) z = height;
		int b = (int)(z*width + x);
		return (hm->Data[b] - t)*hscale;
	};

	// computer vertex xyz
	for (int i = 0; i < height; i++) // z axis
	{

		for (int j = 0; j < width; j++) // x axis
		{
			int b = i * width + j;

			y = (hm->Data[b] - t)*hscale;
			vec3 pos(x, y, z);
			vec2 uv((x + size[0] / 2) / size[0], (z + size[1] / 2) / size[1]);
			//computer normal;

			vec2 P(i, j);
			float hL = heightF(j - 1, i);
			float hR = heightF(j + 1, i);
			float hD = heightF(j, i - 1);
			float hU = heightF(j, i + 1);
			vec3 N(0, 1, 0);
			N.x = hL - hR;
			N.y = 2.0f;
			N.z = hD - hU;
			N = normalize(N);



			DefaultVertex v{ pos,N,uv };
			vertex.push_back(v);
			x += stepsize;
		}
		x = -size[0] / 2;
		z += stepsize;
	}

	return vertex;
}

std::vector<unsigned int> Light::math::GenerateIndicesData(resources::HeightMap * hm,int numSub)
{
	std::size_t numMesh = numSub;			// Num SubMesh device by row and collum
	std::size_t numvert = hm->Width / numMesh;	// Num vertices per SubMesh in Row/Collum
	std::vector<unsigned int> Index;
	std::size_t cnt = 0;
	for (std::size_t i = 0; i < numvert - 1; i++)
		for (std::size_t j = 0; j < numvert - 1; j++)
		{
			Index.push_back(j + (i + 1)*numvert + 1);
			Index.push_back(j + i * numvert + 1);
			Index.push_back(j + i * numvert);

			Index.push_back(j + (i + 1)*numvert);
			Index.push_back(j + (i + 1)*numvert + 1);
			Index.push_back(j + i * numvert);
		}


	return Index;
}
