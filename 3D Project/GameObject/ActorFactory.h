#pragma once
#include "pch.h"

class ActorFactory
{
	ActorId m_lastActorId;

protected:
	GenericObjectFactory<ActorComponent, ComponentId> m_componentFactory;
	std::map<std::string, std::function<ActorComponent*()>> m_ComponentFactory;

public:
	ActorFactory(Scene* pScene);

	// Create Actor from file
	template<class Type = Actor>
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


template<class Type>
Actor * ActorFactory::CreateActor(const char * actorResource, tinyxml2::XMLElement * overrides, const mat4 * initialTransform)
{
	tinyxml2::XMLDocument doc;
	int errorID = doc.LoadFile(actorResource);
	if (errorID)
	{
		E_ERROR("Failed to create Actor from file: " + string(actorResource));
		return nullptr;
	}

	Actor* pActor = new Type(GetNextActorId());

	tinyxml2::XMLElement* pActorData = doc.FirstChildElement("Actor");
	if (!pActor->Init(pActorData))
	{
		E_ERROR("Failed to init Actor:" + string(actorResource));
		return nullptr;
	}

	// Loop through each child element and load the component
	for (tinyxml2::XMLElement* pNode = pActorData->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		ActorComponent* pComponent(VCreateComponent(pNode));
		if (pComponent)
		{
			pComponent->SetOwner(pActor);
			if (!pComponent->VInit(pNode))
			{
				E_ERROR("Component failed to initialize: " + std::string(pNode->Value()));
				continue;
			}
			if (!strcmp(pNode->Value(), "TransformComponent")) pActor->SetTransformComponent(static_cast<TransformComponent*>(pComponent));
			else pActor->AddComponent(pComponent);
			
		}
		else
		{
			E_ERROR("Failed to create Component for actor: " + string(actorResource));
		}
	}


	if (overrides)
	{
		//ModifyActor(pActor, overrides);
	}

	if (initialTransform)
	{
		TransformComponent* pTc = pActor->GetTransform();
		pTc->SetTransform(*initialTransform);
	}

	pActor->PostInit();

	return pActor;
}