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
class SystemUI;
class ActorFactory;
class EffectSystem;
class SoundEngine;
class Context
{
public:

	// debug
	bool DrawSkeleton = 0;

	Context();
	~Context();

	tinyxml2::XMLElement* GetElement(const char* p);

	bool AddSystem(ISubSystem* system);
	template <class T> T* GetSystem();
private:
	ISubSystem * GetSystem(const std::type_info& rtti);

private:

	std::vector<ISubSystem*> m_Systems;
	const char*				ConfigFile = "GameAssets//config.xml";
	tinyxml2::XMLDocument	ConfigData;
};

template <class T>
T* Context::GetSystem()
{
	return static_cast<T*>(GetSystem(typeid(T)));
}