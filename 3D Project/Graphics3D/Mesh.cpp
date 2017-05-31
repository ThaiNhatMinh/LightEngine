#include "..\pch.h"


Mesh::Mesh():m_iVAO(0),m_iEBO(0),m_iVBO(0)
{
	
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_iVAO);
	glDeleteBuffers(1, &m_iVBO);
	glDeleteBuffers(1, &m_iEBO);
}

void Mesh::Finalize(Shader* p)
{
	
	glGenVertexArrays(1, &m_iVAO);
	glGenBuffers(1, &m_iVBO);
	glGenBuffers(1, &m_iEBO);
	

	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER,m_iVBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertexs.size()*sizeof(DefaultVertex), &m_Vertexs[0], GL_STATIC_DRAW);
	
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
