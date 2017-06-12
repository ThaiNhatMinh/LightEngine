#pragma once
#include "..\pch.h"

class Line;
class Debug : public Singleton<Debug>
{
private:
	Line* m_Line;
	Scene* m_pScene;
public:
	Debug(Scene* pScene);
	~Debug();
	virtual void onStartUp() override;
	virtual void onShutDown()override;


	void DrawLine(const vec3& from, const vec3& to, const vec3& color);
};


Debug* gDebug();