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
typedef std::vector<SkeVertex> SkeVertexList;

class SkeMesh :public IMesh
{
public:
	SkeVertexList			m_Vertexs;
	vector<unsigned int>	m_Indices;
	
	
public:
	SkeMesh() { Tex = 0; };
	~SkeMesh() {};
	void Finalize(Shader* p);
};
