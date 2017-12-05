#pragma once
#include "pch.h"

class ActorFactory
{
	ActorId m_lastActorId;
	
protected:
	Context* m_Context;
	std::map<std::string, std::function<ActorComponent*()>> m_ComponentFactoryMap;
	std::map<std::string, std::function<Actor*(int id)>> m_ActorFactoryMap;

	void EventCreateWeapon(std::shared_ptr<const IEvent> pEvents);
public:
	ActorFactory(Context*,Scene* pScene);
	~ActorFactory();
	// Create Actor from file
	
	Actor* CreateActor(const char* actorResource, tinyxml2::XMLElement* overrides, const mat4* initialTransform);

	// Create Actor with primitive shape. Not support now. Need to load from file
	// [TODO]- Load from file
	//Actor* CreateActor(const char* name,ShapeType type, const mat4& initialTransform);


	void ModifyActor(Actor* pActor, tinyxml2::XMLElement* overrides);

	//protected:
	// [rez] This function can be overridden by a subclass so you can create game-specific C++ components.  If you do
	// this, make sure you call the base-class version first.  If it returns NULL, you know it's not an engine component.
	virtual ActorComponent* VCreateComponent(const tinyxml2::XMLElement* pData);

private:
	ActorId GetNextActorId(void) { ++m_lastActorId; return m_lastActorId; }
};

