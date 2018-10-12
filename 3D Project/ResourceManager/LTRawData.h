#pragma once
#include "../Utilities/LTBStruct.h"


class LTRawMesh
{
public:
	std::vector<SkeVertex>				Vertexs;
	std::vector<unsigned int>				Indices;
	std::string								Name;
};


class LTRawData
{
public:
	std::vector<LTRawMesh>		Meshs;
	std::vector<Light::SkeNode>		SkeNodes;
	std::vector<Light::WeightBlend>	wb;
	std::vector<std::string>	ChildName;
	std::vector<Light::Animation>		Anims;
	std::vector<Light::LTBSocket>		Sockets;
};