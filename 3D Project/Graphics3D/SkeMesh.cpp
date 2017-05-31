#include "..\pch.h"

void SkeMesh::Finalize(Shader * p)
{
	glGenVertexArrays(1, &m_iVAO);
	glGenBuffers(1, &m_iVBO);
	glGenBuffers(1, &m_iEBO);


	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertexs.size()*sizeof(SkeVertex), &m_Vertexs[0], GL_STATIC_DRAW);

	size_t stride = sizeof(SkeVertex);


	size_t offset = 0;

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

	location = p->GetAttribLocation("ws1");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += sizeof(vec2);

	location = p->GetAttribLocation("ws2");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += sizeof(vec2);

	location = p->GetAttribLocation("ws3");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += sizeof(vec2);

	location = p->GetAttribLocation("ws4");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);

}
