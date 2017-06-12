#pragma once
#include "..\..\pch.h"


class Line
{
private:
	unsigned int			VAO;
	unsigned int			VBO;
public:
	Line();
	~Line() {};
	void Draw(const vec3& from, const vec3& to);
};