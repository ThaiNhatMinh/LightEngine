#pragma once
#include "pch.h"
#include "Graphics3D\OpenGLRenderer.h"
class Application : public IApplication
{
public:
	Application() {};
	~Application();
	virtual void Setup() {};
	virtual void Start() {};

	Debug& GetDebug();
	
	void MainLoop();
private:
	bool m_bRunMainLoop;
	void SetupSubmodule();
	Scene* m_pScene;
	Actor* pp;
	int anim;
};