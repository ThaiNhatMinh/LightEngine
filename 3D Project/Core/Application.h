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
	
	std::unique_ptr<Windows>			m_pWindows;
	std::unique_ptr<OpenGLRenderer>		m_pRenderer;
	std::unique_ptr<SystemUI>			m_pSystemUI;
	std::unique_ptr<Console>			m_pConsole;

	std::unique_ptr<Debug>				m_pDebuger;
	std::unique_ptr<EventManager>		m_pEventManager;
	std::unique_ptr<GameTimer>			m_pTimer;
	std::unique_ptr<Resources>			m_pResources;
	std::unique_ptr<DirectInput>		m_pInput;
	std::unique_ptr<BulletPhysics>		m_pPhysic;
	std::unique_ptr<ActorFactory>		m_pActorFactory;
	std::unique_ptr<EffectSystem>		m_pEffectSystem;
	std::unique_ptr<SoundEngine>		m_pSoundEngine;
	std::unique_ptr<VGUI>				m_pVGUI;

	std::unique_ptr<Context> m_Context;
	std::unique_ptr<IGame> m_Game;
	
	int m_DebugPhysic;
};