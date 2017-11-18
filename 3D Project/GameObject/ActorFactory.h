#pragma once
#include "pch.h"

class ActorFactory: public Singleton<ActorFactory>
{
	ActorId m_lastActorId;

protected:
	GenericObjectFactory<ActorComponent, ComponentId> m_componentFactory;

public:
	void onStartUp();

	// Create Actor from file
	Actor* CreateActor(const char* actorResource, tinyxml2::XMLElement* overrides, const mat4* initialTransform);
	// Create Actor with primitive shape
	Actor* CreateActor(const char* name,ShapeType type, const mat4& initialTransform);
	void ModifyActor(Actor* pActor, tinyxml2::XMLElement* overrides);

	//protected:
	// [rez] This function can be overridden by a subclass so you can create game-specific C++ components.  If you do
	// this, make sure you call the base-class version first.  If it returns NULL, you know it's not an engine component.
	virtual ActorComponent* VCreateComponent(tinyxml2::XMLElement* pData);

private:
	ActorId GetNextActorId(void) { ++m_lastActorId; return m_lastActorId; }
};

ActorFactory* gActorFactory();