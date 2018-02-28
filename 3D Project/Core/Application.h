#pragma once
#include "pch.h"
#include "Graphics3D\OpenGLRenderer.h"
#include "GamePlugin\GamePluginManager.h"
class Application : public IApplication
{
public:
	Application();
	~Application();
	

	
	
	void MainLoop();
private:
	bool m_bRunMainLoop;
	//void SetupSubmodule();
protected:
	virtual void Start() {};
	
	Context			m_Context;
	std::unique_ptr<Windows>			m_Windows;
	std::unique_ptr<DirectInput>		m_Input;
	std::unique_ptr<OpenGLRenderer>	m_Renderer;
	std::unique_ptr<EventManager>	m_EventManager;
	std::unique_ptr<ActorFactory>	m_Factory;
	std::unique_ptr<SoundEngine>		m_SoundEngine;
	std::unique_ptr<Resources>		m_Resources;
	std::unique_ptr<SystemUI>		m_SystemUI;
	std::unique_ptr<Console>			m_Console;
	std::unique_ptr<Debug>			m_DebugRender;
	std::unique_ptr<BulletPhysics>	m_Physic;
	std::unique_ptr<GameTimer>		m_Timer;
	std::unique_ptr<EffectSystem>	m_EffectSystem;
	std::unique_ptr<VGUI>			m_VGUI;

	GamePluginManager m_GamePlugins;
	
	std::unique_ptr<IGame> m_Game;
	
	
	int m_DebugPhysic;
};