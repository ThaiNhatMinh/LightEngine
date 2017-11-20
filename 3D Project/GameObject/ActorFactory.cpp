#include "pch.h"

ActorFactory::ActorFactory(Scene* pScene)
{
	m_lastActorId = 1;

	//m_componentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::Name));
	//m_componentFactory.Register<MeshRenderComponent>(ActorComponent::GetIdFromName(MeshRenderComponent::Name));
	//m_componentFactory.Register<PhysicsComponent>(ActorComponent::GetIdFromName(PhysicsComponent::Name));
	//m_componentFactory.Register<ColliderComponent>(ActorComponent::GetIdFromName(ColliderComponent::Name));
	//m_componentFactory.Register<RigidBodyComponent>(ActorComponent::GetIdFromName(RigidBodyComponent::Name));
	//m_componentFactory.Register<AnimationComponent>(ActorComponent::GetIdFromName(AnimationComponent::Name));
	//m_componentFactory.Register<ScriptComponent>(ActorComponent::GetIdFromName(ScriptComponent::Name));
	//m_componentFactory.Register<CharacterControllerComponent>(ActorComponent::GetIdFromName(CharacterControllerComponent::Name));
	//m_componentFactory.Register<CameraComponent>(ActorComponent::GetIdFromName(CameraComponent::Name));

	m_ComponentFactory.insert(std::make_pair(TransformComponent::Name, []() { return new TransformComponent(); } ));
	m_ComponentFactory.insert(std::make_pair(MeshRenderComponent::Name, []() { return new MeshRenderComponent(); }));
	m_ComponentFactory.insert(std::make_pair(ColliderComponent::Name, []() { return new ColliderComponent(); }));
	m_ComponentFactory.insert(std::make_pair(RigidBodyComponent::Name, []() { return new RigidBodyComponent(); }));
	m_ComponentFactory.insert(std::make_pair(AnimationComponent::Name, []() { return new AnimationComponent(); }));
	m_ComponentFactory.insert(std::make_pair(CharacterControllerComponent::Name, []() { return new CharacterControllerComponent(); }));
	m_ComponentFactory.insert(std::make_pair(LogicComponent::Name, []() { return new LogicComponent(); }));
	m_ComponentFactory.insert(std::make_pair(CameraComponent::Name, [pScene]()
		{ 
			auto a = new CameraComponent();
			pScene->SetCamera(a);
			return a; }));
	
	
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

	pActor->PostInit();

	return pActor;
}

Actor * ActorFactory::CreateActor(const char * name, ShapeType type, const mat4& initialTransform)
{

	Actor* pActor = new Actor(GetNextActorId());
	pActor->VSetName(name);

	// Create mesh renderer
	
	vector<IMesh*> v;
	v.push_back(gResources()->CreateShape(SHAPE_BOX));

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
	auto factory = m_ComponentFactory.find(name);
	ActorComponent* pComponent(factory->second());
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

	
	return pComponent;
}

