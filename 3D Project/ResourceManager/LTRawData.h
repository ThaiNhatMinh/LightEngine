#pragma once
#include "..\Graphics3D\SkeMesh.h"
#include "LTModel.h"

using namespace Light;

class LTRawMesh
{
public:
	Light::SkeMesh::SkeVertexList			Vertexs;
	Light::SkeMesh::IndicesList				Indices;
	std::string								Name;
};


class LTRawData
{
public:
	std::vector<LTRawMesh>		Meshs;
	std::vector<SkeNode>		SkeNodes;
	std::vector<WeightBlend>	wb;
	std::vector<std::string>	ChildName;
	std::vector<Animation>		Anims;
	std::vector<LTBSocket>		Sockets;
};