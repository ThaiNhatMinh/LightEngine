#include "..\pch.h"


Mesh::Mesh()
{
	Tex = 0;
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::Finalize(Shader* p)
{
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertexs.size()*sizeof(DefaultVertex), &m_Vertexs[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

	size_t stride = sizeof(DefaultVertex);
	

	size_t offset=0;

	GLuint location = p->GetAttribLocation("position");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 3 * sizeof(float);
	
	
	location = p->GetAttribLocation("normal");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 3 * sizeof(float);
	
	
	
	location = p->GetAttribLocation("uv");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 2 * sizeof(float);
	location++;
	

	glBindVertexArray(0);
}

void Mesh::Scale(vec3 scale)
{
	for (size_t i = 0; i < m_Vertexs.size(); i++)
	{
		m_Vertexs[i].pos.x *= scale.x;
		m_Vertexs[i].pos.y *= scale.y;
		m_Vertexs[i].pos.z *= scale.z;
	}
}
