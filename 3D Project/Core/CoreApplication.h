#pragma once
#include "pch.h"
#include "..\GameLogic\BaseGameLogic.h"

class CoreApplication : public IApplication
{
public:
	CoreApplication() {};
	~CoreApplication();
	virtual void Setup() {};
	virtual void Start() {};

	const Debug& GetDebug();
	
	void MainLoop();
private:
	bool m_bRunMainLoop;
	void SetupSubmodule();
	Scene* m_pScene;
};