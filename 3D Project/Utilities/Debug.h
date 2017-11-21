#pragma once
#include "pch.h"
#include "Utilities\PrimitiveShape\Line.h"

class DebugData
{
public:
	glm::mat4 MVP;
	glm::vec3 color;
	vec3 data[2];
};

class Debug
{
private:
	Scene* m_pScene;
	Shader* pShader;
	unsigned int			VAO;
	unsigned int			VBO;
	std::vector<DebugData> m_Lists;
public:
	Debug(Scene* pScene);
	~Debug();

	void DrawLine(const vec3& from, const vec3& to, const vec3& color,const mat4& m = mat4(1.0f));
	void Render();
};
