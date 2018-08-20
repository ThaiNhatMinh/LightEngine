#pragma once
#include "..\Graphics3D\SkeMesh.h"

class LTRawData
{
public:
	Light::SkeMesh::SkeVertexList			Vertexs;
	Light::SkeMesh::IndicesList				Indices;
	std::string								Name;
};