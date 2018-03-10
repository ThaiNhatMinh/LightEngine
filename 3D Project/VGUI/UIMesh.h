#pragma once


class UIMesh
{
public:
	UIMesh();
	~UIMesh();

	VertexArray VAO;
	BufferObject VBO;
	UIMesh(UIMesh&& other);
};