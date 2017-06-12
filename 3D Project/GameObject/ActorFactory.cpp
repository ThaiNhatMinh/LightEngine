#include "..\pch.h"

void ActorFactory::onStartUp(void)
{
	m_lastActorId = 1;

	m_componentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::Name));
	m_componentFactory.Register<MeshRenderComponent>(ActorComponent::GetIdFromName(MeshRenderComponent::Name));
	m_componentFactory.Register<PhysicsComponent>(ActorComponent::GetIdFromName(PhysicsComponent::Name));

}

Actor * ActorFactory::CreateActor(const char * actorResource, tinyxml2::XMLElement * overrides, const mat4 * initialTransform)
{
	tinyxml2::XMLDocument doc;
	int errorID = doc.LoadFile(actorResource);
	if (errorID)
	{
		E_ERROR("Failed to create Actor from file: " + string(actorResource));
		return nullptr;
	}

	Actor* pActor = new Actor(GetNextActorId());

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
			pActor->AddComponent(pComponent);
			pComponent->SetOwner(pActor);
		}
		else
		{
			E_ERROR("Failed to create Component for actor: " + string(actorResource));
		}
	}


	if (overrides)
	{
		ModifyActor(pActor, overrides);
	}

	if (initialTransform)
	{
		TransformComponent* pTc = pActor->GetComponent<TransformComponent>("TransformComponent");
		pTc->SetTransform(*initialTransform);
	}

	return pActor;
}

Actor * ActorFactory::CreateActor(const char * name, ShapeType type, const mat4& initialTransform)
{

	Actor* pActor = new Actor(GetNextActorId());
	pActor->VSetName(name);

	// Create mesh renderer
	
	vector<IMesh*> v;
	v.push_back(gResources()->CreateShape(CUBE));

	ActorComponent* pMeshRenderC = new MeshRenderComponent(v);
	pActor->AddComponent(pMeshRenderC);
	pMeshRenderC->SetOwner(pActor);

	// Create Transform
	ActorComponent* pTransformC = new TransformComponent(initialTransform);
	pActor->AddComponent(pTransformC);
	pTransformC->SetOwner(pActor);

	//pActor->PostInit();

	return pActor;
}

void ActorFactory::ModifyActor(Actor * pActor, tinyxml2::XMLElement * overrides)
{
}

ActorComponent * ActorFactory::VCreateComponent(tinyxml2::XMLElement * pData)
{
	const char* name = pData->Value();
	ActorComponent* pComponent(m_componentFactory.Create(ActorComponent::GetIdFromName(name)));
	// initialize the component if we found one
	if (pComponent)
	{
		if (!pComponent->VInit(pData))
		{
			E_ERROR("Component failed to initialize: " + std::string(name));
			delete pComponent;
			return nullptr;
		}
	}
	else
	{
		E_ERROR("Couldn't find ActorComponent named " + std::string(name));
		return nullptr;  // fail
	}

	// pComponent will be NULL if the component wasn't found.  This isn't necessarily an error since you might have a 
	// custom CreateComponent() function in a sub class.
	return pComponent;
}

ActorFactory * gActorFactory()
{
	return ActorFactory::InstancePtr();
}
