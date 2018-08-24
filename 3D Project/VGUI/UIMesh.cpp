#include <pch.h>

UIMesh::UIMesh():VAO(),VBO(GL_ARRAY_BUFFER)
{
	VAO.Bind();
	VBO.Bind();

	VBO.SetData(sizeof(float) * 4, NULL, GL_STATIC_DRAW);
	VAO.SetAttibutePointer(SHADER_POSITION_ATTRIBUTE, 4, GL_FLOAT, sizeof(float) * 4, 0);
	VAO.UnBind();
	VBO.UnBind();
}

UIMesh::~UIMesh()
{
}

UIMesh::UIMesh(UIMesh && other):VAO(std::move(other.VAO)),VBO(std::move(VBO))
{
	
}
