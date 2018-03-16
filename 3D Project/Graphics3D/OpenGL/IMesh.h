#pragma once
// mesh interface
#include "BufferObject.h"
#include "VertexArray.h"
#include "..\Material.h"
class IMesh
{
public:
	// Use to generate Vertex Buffer Object, Vertex Array Object.
	IMesh() :VAO(), VBO(GL_ARRAY_BUFFER), EBO(GL_ELEMENT_ARRAY_BUFFER) {}
	virtual ~IMesh() {};
	string					Name;
	VertexArray				VAO;
	BufferObject			VBO, EBO;
	GLuint					NumIndices;
	Texture*				Tex;
	GLuint					Topology;
	Material				mat;
};