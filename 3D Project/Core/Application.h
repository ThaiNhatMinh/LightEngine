#pragma once
#include "pch.h"
#include "Graphics3D\OpenGLRenderer.h"
class Application : public IApplication
{
public:
	Application():m_DebugPhysic(0){};
	~Application();
	

	
	
	void MainLoop();
private:
	bool m_bRunMainLoop;
	void SetupSubmodule();
protected:
	virtual void Setup() {};
	virtual void Start() {};
	//Scene* m_pScene;

	std::unique_ptr<Context> m_Context;
	std::unique_ptr<IGame> m_Game;
	
	int m_DebugPhysic;
};