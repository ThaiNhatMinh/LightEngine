#include "pch.h"

CubeMesh::CubeMesh(float sizex , float sizey, float sizez)
{
	m_Vertexs.resize(24);

	// +Z side
	m_Vertexs[0].pos = vec3(-1.0f, -1.0f, 1.0f);
	m_Vertexs[1].pos = vec3(1.0f, -1.0f, 1.0f);
	m_Vertexs[2].pos = vec3(1.0f, 1.0f, 1.0f);
	m_Vertexs[3].pos = vec3(-1.0f, 1.0f, 1.0f);
	m_Vertexs[0].normal = vec3(0, 0, -1.0);
	m_Vertexs[1].normal = vec3(0, 0, -1.0);
	m_Vertexs[2].normal = vec3(0, 0, -1.0);
	m_Vertexs[3].normal = vec3(0, 0, -1.0);
	m_Vertexs[0].uv = vec2(0, 0);
	m_Vertexs[1].uv = vec2(1,0);
	m_Vertexs[2].uv = vec2(1,1);
	m_Vertexs[3].uv = vec2(0,1);

	// -Z side
	m_Vertexs[4].pos = vec3(-1.0f, -1.0f, -1.0f);
	m_Vertexs[5].pos = vec3(-1.0f, 1.0f, -1.0f);
	m_Vertexs[6].pos = vec3(1.0f, 1.0f, -1.0f);
	m_Vertexs[7].pos = vec3(1.0f, -1.0f, -1.0f);
	m_Vertexs[4].normal = vec3(0, 0, 1.0);
	m_Vertexs[5].normal = vec3(0, 0, 1.0);
	m_Vertexs[6].normal = vec3(0, 0, 1.0);
	m_Vertexs[7].normal = vec3(0, 0, 1.0);
	m_Vertexs[4].uv = vec2(0, 0);
	m_Vertexs[5].uv = vec2(1, 0);
	m_Vertexs[6].uv = vec2(1, 1);
	m_Vertexs[7].uv = vec2(0, 1);

	// +X side
	m_Vertexs[8].pos = vec3(1.0f, -1.0f, -1.0f);
	m_Vertexs[9].pos = vec3(1.0f, 1.0f, -1.0f);
	m_Vertexs[10].pos = vec3(1.0f, 1.0f, 1.0f);
	m_Vertexs[11].pos = vec3(1.0f, -1.0f, 1.0f);
	m_Vertexs[8].normal = vec3(-1.0f,0,0);
	m_Vertexs[9].normal = vec3(-1.0f, 0, 0);
	m_Vertexs[10].normal = vec3(-1.0f, 0, 0);
	m_Vertexs[11].normal = vec3(-1.0f, 0, 0);
	m_Vertexs[8].uv = vec2(0, 0);
	m_Vertexs[9].uv = vec2(1, 0);
	m_Vertexs[10].uv = vec2(1, 1);
	m_Vertexs[11].uv = vec2(0, 1);

	// -X side
	m_Vertexs[12].pos = vec3(-1.0f, -1.0f, -1.0f);
	m_Vertexs[13].pos = vec3(-1.0f, -1.0f, 1.0f);
	m_Vertexs[14].pos = vec3(-1.0f, 1.0f, 1.0f);
	m_Vertexs[15].pos = vec3(-1.0f, 1.0f, -1.0f);
	m_Vertexs[12].normal = vec3(1.0f, 0, 0);
	m_Vertexs[13].normal = vec3(1.0f, 0, 0);
	m_Vertexs[14].normal = vec3(1.0f, 0, 0);
	m_Vertexs[15].normal = vec3(1.0f, 0, 0);
	m_Vertexs[12].uv = vec2(0, 0);
	m_Vertexs[13].uv = vec2(1, 0);
	m_Vertexs[14].uv = vec2(1, 1);
	m_Vertexs[15].uv = vec2(0, 1);

	// +Y side
	m_Vertexs[16].pos = vec3(-1.0f, -1.0f, -1.0f);
	m_Vertexs[17].pos = vec3(1.0f, -1.0f, -1.0f);
	m_Vertexs[18].pos = vec3(1.0f, -1.0f, 1.0f);
	m_Vertexs[19].pos = vec3(-1.0f, -1.0f, 1.0f);
	m_Vertexs[16].normal = vec3(0,-1.0f, 0);
	m_Vertexs[17].normal = vec3(0, -1.0f, 0);
	m_Vertexs[18].normal = vec3(0, -1.0f, 0);
	m_Vertexs[19].normal = vec3(0, -1.0f, 0);
	m_Vertexs[16].uv = vec2(0, 0);
	m_Vertexs[17].uv = vec2(1, 0);
	m_Vertexs[18].uv = vec2(1, 1);
	m_Vertexs[19].uv = vec2(0, 1);

	// -Y side
	m_Vertexs[20].pos = vec3(-1.0f, 1.0f, -1.0f);
	m_Vertexs[21].pos = vec3(-1.0f, 1.0f, 1.0f);
	m_Vertexs[22].pos = vec3(1.0f, 1.0f, 1.0f);
	m_Vertexs[23].pos = vec3(1.0f, 1.0f, -1.0f);
	m_Vertexs[20].normal = vec3(0, 1.0f, 0);
	m_Vertexs[21].normal = vec3(0, 1.0f, 0);
	m_Vertexs[22].normal = vec3(0, 1.0f, 0);
	m_Vertexs[23].normal = vec3(0, 1.0f, 0);
	m_Vertexs[20].uv = vec2(0, 0);
	m_Vertexs[21].uv = vec2(1, 0);
	m_Vertexs[22].uv = vec2(1, 1);
	m_Vertexs[23].uv = vec2(0, 1);

	m_Indices = vector<unsigned int>{
		0,1,2,0,2,3,
		4,5,6,4,6,7,
		8,9,10,8,10,11,
		12,13,14,12,14,15,
		16,17,18,16,18,19,
		20,21,22,20,22,23};

	Topology = GL_TRIANGLES;

	NumIndices = m_Indices.size();

	for (size_t i = 0; i < m_Vertexs.size(); i++)
	{
		m_Vertexs[i].pos.x *= sizex;
		m_Vertexs[i].pos.y *= sizey;
		m_Vertexs[i].pos.z *= sizez;
	}


	VAO.Bind();
	VBO.Bind();
	EBO.Bind();

	VBO.SetData(m_Vertexs.size() * sizeof(DefaultVertex), &m_Vertexs[0], GL_STATIC_DRAW);
	EBO.SetData(m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);


	size_t stride = sizeof(DefaultVertex);


	size_t offset = 0;


	VAO.SetAttibutePointer(SHADER_POSITION_ATTRIBUTE, 3, GL_FLOAT, stride, offset);
	offset += 3 * sizeof(float);

	VAO.SetAttibutePointer(SHADER_NORMAL_ATTRIBUTE, 3, GL_FLOAT, stride, offset);
	offset += 3 * sizeof(float);

	VAO.SetAttibutePointer(SHADER_TEXCOORD_ATTRIBUTE, 2, GL_FLOAT, stride, offset);
	offset += 2 * sizeof(float);

	NumIndices = m_Indices.size();
	Topology = GL_TRIANGLES;
}
