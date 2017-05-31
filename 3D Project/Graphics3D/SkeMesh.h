#pragma once
#include "..\pch.h"

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

class SkeMesh 
{
public:
	string					m_Name;
	unsigned int			m_iVAO;
	unsigned int			m_iVBO;
	unsigned int			m_iEBO;
	SkeVertexList			m_Vertexs;
	vector<unsigned int>	m_Indices;
	Material				m_Material; // every mesh has a material
	GLuint					Topology;
	Texture*				m_pTexture; // If there is no texture. It will use color
public:
	SkeMesh() {};
	~SkeMesh() {};
	void Finalize(Shader* p);
};
