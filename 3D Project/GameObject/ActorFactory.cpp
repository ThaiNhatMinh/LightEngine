#include "pch.h"

ActorFactory::ActorFactory(Scene* pScene)
{
	m_lastActorId = 1;
	m_ComponentFactoryMap.insert(std::make_pair(TransformComponent::Name, []() { return new TransformComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair(ColliderComponent::Name, []() { return new ColliderComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair(RigidBodyComponent::Name, []() { return new RigidBodyComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair(AnimationComponent::Name, []() { return new AnimationComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair(PVAnimationComponent::Name, []() { return new PVAnimationComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair(CharacterControllerComponent::Name, []() { return new CharacterControllerComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair(LogicComponent::Name, []() { return new LogicComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair(MeshRenderComponent::Name, []() { return new MeshRenderComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair(TerrainRenderComponent::Name, []() { return new TerrainRenderComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair(CameraComponent::Name, [pScene]()
		{ 
			auto a = new CameraComponent();
			pScene->SetCamera(a);
			return a; }));
	
	m_ActorFactoryMap.insert(std::make_pair("Player", [](int id) {return new Player(id); }));
	m_ActorFactoryMap.insert(std::make_pair("World", [](int id) {return new TerrainWorld(id); }));
	m_ActorFactoryMap.insert(std::make_pair("PlayerView", [](int id) {return new PlayerView(id); }));
	
}

/*
Actor * ActorFactory::CreateActor(const char * name, ShapeType type, const mat4& initialTransform)
{

	Actor* pActor = new Actor(GetNextActorId());
	pActor->VSetName(name);

	// Create mesh renderer
	
	vector<IMesh*> v;
	v.push_back(m_Context->m_pResources->CreateShape(SHAPE_BOX));

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
*/
void ActorFactory::ModifyActor(Actor * pActor, tinyxml2::XMLElement * overrides)
{
}


ActorComponent * ActorFactory::VCreateComponent(const tinyxml2::XMLElement* pData)
{
	const char* name = pData->Value();
	ActorComponent* pComponent;
	auto factory = m_ComponentFactoryMap.find(name);
	if(factory!= m_ComponentFactoryMap.end()) pComponent = factory->second();
	else return nullptr;
	// initialize the component if we found one
	if (pComponent)
	{
		
	}
	else
	{
		E_ERROR("Couldn't find ActorComponent named " + std::string(name));
		return nullptr;  // fail
	}

	
	return pComponent;
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

	tinyxml2::XMLElement* pActorData = doc.FirstChildElement("Actor");

	const char* type = pActorData->Attribute("type");
	auto factory = m_ActorFactoryMap.find(type);
	Actor* pActor = nullptr;
	if(factory != m_ActorFactoryMap.end()) pActor = factory->second(GetNextActorId());
	else pActor = new Actor(GetNextActorId());

	
	if (!pActor->Init(pActorData))
	{
		E_ERROR("Failed to init Actor:" + string(actorResource));
		return nullptr;
	}

	tinyxml2::XMLElement* pComponentData = pActorData->FirstChildElement("Component");
	// Loop through each child element and load the component
	for (tinyxml2::XMLElement* pNode = pComponentData->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
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