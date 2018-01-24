#pragma once
#include "pch.h"
#include "Utilities\PrimitiveShape\Line.h"

class DebugData
{
public:
	glm::vec3 pos;
	glm::vec3 color;
	
};
class CameraComponent;
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

	void DrawLine(const vec3& from, const vec3& to, const vec3& color, const mat4& m = mat4());
	void DrawLineBox(vec3 min, vec3 max, vec3 color = vec3(0.5f), const mat4& m = mat4());
	void DrawCoord(const mat4& m);
	void Render();
};
