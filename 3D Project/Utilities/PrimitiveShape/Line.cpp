#include "..\..\pch.h"


Line::Line()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,2*sizeof(vec3), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)0);

	glBindVertexArray(0);


}

void Line::Draw(const vec3 & from, const vec3 & to)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	vec3 data[2] = { from,to };

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);
	glDrawArrays(GL_LINES, 0, 2);


}
