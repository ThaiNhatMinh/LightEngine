#pragma once
#include "pch.h"
#include "Utilities\PrimitiveShape\Line.h"

class DebugData
{
public:
	glm::vec3 pos[2];
	glm::vec3 color;
	
};

class Debug: public ISubSystem
{
private:
	mat4 m_View;
	mat4 m_Proj;
	mat4 VP;
	Shader* pShader;
	unsigned int			VAO;
	unsigned int			VBO;
	std::vector<DebugData> m_Lists;
public:
	Debug();
	virtual void Init(Context* c)override;
	virtual void ShutDown()override;
	void SetView(CameraComponent* cam);
	~Debug();

	void DrawLine(const vec3& from, const vec3& to, const vec3& color,const mat4& m = mat4(1.0f));
	void Render();
};
