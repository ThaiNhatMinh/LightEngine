#pragma once
#include "..\pch.h"

struct DefaultVertex
{
	vec3 pos;
	vec3 normal;
	vec2 uv;
};

class Mesh: public IMesh 
{
public:
	vector<DefaultVertex>	m_Vertexs;
	vector<unsigned int>	m_Indices;
public:
	Mesh();
	~Mesh();

	// dirive class can redifine this function to add more data;
	virtual void Finalize(Shader* p);
	virtual void Scale(vec3 scale);
};

