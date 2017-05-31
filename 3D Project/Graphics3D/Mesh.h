#pragma once
#include "..\pch.h"

struct DefaultVertex
{
	vec3 pos;
	vec3 normal;
	vec2 uv;
};

class Mesh
{
public:
	unsigned int			m_iVAO;
	unsigned int			m_iVBO;
	unsigned int			m_iEBO;
	vector<DefaultVertex>	m_Vertexs;
	vector<unsigned int>	m_Indices;
	Material				m_Material; // every mesh has a material
	GLuint					Topology;
	Texture*				m_pTexture; // If there is no texture. It will use color
public:
	Mesh();
	~Mesh();

	// dirive class can redifine this function to add more data;
	virtual void Finalize(Shader* p);
	
};

