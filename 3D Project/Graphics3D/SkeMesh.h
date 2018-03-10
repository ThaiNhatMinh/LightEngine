#pragma once
#include "pch.h"

struct Weight
{
	Weight(int b, float w) { Bone = (float)b; weight = w; };
	Weight():Bone(0), weight(0){};
	float Bone;
	float weight;
};

typedef vector<Weight> WeightList;
struct SkeVertex
{
	vec3		pos;
	vec3		normal;
	vec2		uv;
	Weight		weights[4];
	
};


class SkeMesh :public IMesh
{
public:
	typedef std::vector<SkeVertex> SkeVertexList;
	typedef std::vector<unsigned int> IndicesList;
private:
	
	SkeVertexList			m_Vertexs;
	IndicesList	m_Indices;
	
	
public:
	SkeMesh(const SkeVertexList& vertex, const IndicesList indices);
	~SkeMesh() {};
	const SkeVertexList& GetVertexs();
	const IndicesList& GetIndices();
};
