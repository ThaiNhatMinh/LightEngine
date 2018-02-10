#include "pch.h"

void UIMesh::Init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 2, NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(SHADER_POSITION_ATTRIBUTE);
	glVertexAttribPointer(SHADER_POSITION_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (GLvoid*)0);
	glEnableVertexAttribArray(SHADER_TEXCOORD_ATTRIBUTE);
	glVertexAttribPointer(SHADER_TEXCOORD_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)(sizeof(float)*2));
}

void UIMesh::Shutdown()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
