#pragma once
#include "pch.h"
#include "Utilities\PrimitiveShape\Line.h"

class Line;
class Debug
{
private:
	Line m_Line;
	Scene* m_pScene;
public:
	Debug(Scene* pScene);
	~Debug();

	void DrawLine(const vec3& from, const vec3& to, const vec3& color)const;
};
