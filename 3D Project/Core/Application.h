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

	
	
	void MainLoop();
private:
	bool m_bRunMainLoop;
	void SetupSubmodule();
	//Scene* m_pScene;
	
	std::unique_ptr<Context> m_Context;
	
};