#pragma once

class Mesh :public IMesh
{
public:

	string					Name;
	VertexArray				VAO;
	BufferObject			VBO, EBO;
	GLuint					NumIndices;
	Texture*				Tex;
	GLuint					Topology;
	Material				mat;
};