#pragma once

#include "OpenGL\IMesh.h"
struct DefaultVertex
{
	vec3 pos;
	vec3 normal;
	vec2 uv;
};

class Mesh: public IMesh 
{
protected:
	vector<DefaultVertex>	m_Vertexs;
	vector<unsigned int>	m_Indices;
public:
	Mesh(const std::vector<DefaultVertex>& vertex,const std::vector<unsigned int> indices);
	~Mesh();
	GLuint GetNumVertex();
	GLuint GetNumIndices();

	GLvoid* GetVertexData();
	GLvoid* GetIndicesData();

protected:
	Mesh()=default;
};

struct imguiVertex
{
	vec2 pos;
	vec2 uv;
	vec4 color;
};

class imguiMesh : public IMesh
{
private:
	vector<DefaultVertex>	m_Vertexs;
	vector<unsigned int>	m_Indices;
public:
	imguiMesh();
	~imguiMesh();

};

