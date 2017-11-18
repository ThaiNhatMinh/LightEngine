#pragma once

#include "pch.h"

class BaseGameLogic
{
private:
	Scene*		m_pScene;
	Windows*	m_pWindows;

public:

	virtual void Init();
	virtual void ShutDown();

	virtual void Update();
	virtual void Render();

	virtual void GetScene();
	virtual void GetWindows();

	
};