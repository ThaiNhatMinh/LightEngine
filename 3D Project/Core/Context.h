#pragma once
#include <pch.h>

class Console;
class Windows;
class OpenGLRenderer;
class EventManager;
class GameTimer;
class Resources;
class DirectInput;
class BulletPhysics;

class Context
{
public:
	std::unique_ptr<Windows>			m_pWindows;
	std::unique_ptr<OpenGLRenderer>		m_pRenderer;
	std::unique_ptr<EventManager>		m_pEventManager;
	std::unique_ptr<GameTimer>			m_pTimer;
	std::unique_ptr<Resources>			m_pResources;
	std::unique_ptr<DirectInput>		m_pInput;
	std::unique_ptr<BulletPhysics>		m_pPhysic;
	std::unique_ptr<Console>			m_pConsole;
	


	Context();
	~Context();

	tinyxml2::XMLElement* GetElement(const char* p);
private:
	const char*				ConfigFile = "GameAssets//config.xml";
	tinyxml2::XMLDocument	ConfigData;
};