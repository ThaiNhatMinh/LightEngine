#pragma once
#include "pch.h"
#include "..\GameLogic\BaseGameLogic.h"

class CoreApplication : public Singleton<CoreApplication>
{
public:

	virtual void onStartUp();
	virtual void onShutDown();

	// This only set one when everything begin
	virtual void SetGameLogic(BaseGameLogic* pGameLogic);
	
	bool MainLoop();
private:
	bool m_bRunMainLoop;

	BaseGameLogic* m_pGameLogic;
	//Actor *p2,*p3;
	Scene* m_pScene;
};