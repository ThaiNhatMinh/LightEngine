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

class Context: public IContext
{
public:
	Context();
	~Context();

	virtual ISubSystem* GetSystem(const type_info&);
	virtual void AddSystem(ISubSystem* system) ;
	template<class T>T* GetSystem();

	tinyxml2::XMLElement* GetElement(const char* p);
private:
	const char*				ConfigFile = "GameAssets//config.xml";
	tinyxml2::XMLDocument	ConfigData;

	std::map<std::string,ISubSystem*> m_Systems;
};

template<class T>T* Context::GetSystem()
{
	return static_cast<T*>(GetSystem(typeid(T)));
}