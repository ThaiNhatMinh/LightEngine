#include "pch.h"

ActorFactory::ActorFactory(Scene* pScene)
{
	m_lastActorId = 1;
	m_ComponentFactory.insert(std::make_pair(TransformComponent::Name, []() { return new TransformComponent(); }));
	m_ComponentFactory.insert(std::make_pair(ColliderComponent::Name, []() { return new ColliderComponent(); }));
	m_ComponentFactory.insert(std::make_pair(RigidBodyComponent::Name, []() { return new RigidBodyComponent(); }));
	m_ComponentFactory.insert(std::make_pair(AnimationComponent::Name, []() { return new AnimationComponent(); }));
	m_ComponentFactory.insert(std::make_pair(CharacterControllerComponent::Name, []() { return new CharacterControllerComponent(); }));
	m_ComponentFactory.insert(std::make_pair(LogicComponent::Name, []() { return new LogicComponent(); }));
	m_ComponentFactory.insert(std::make_pair(MeshRenderComponent::Name, []() { return new MeshRenderComponent(); }));
	m_ComponentFactory.insert(std::make_pair(TerrainRenderComponent::Name, []() { return new TerrainRenderComponent(); }));
	m_ComponentFactory.insert(std::make_pair(CameraComponent::Name, [pScene]()
		{ 
			auto a = new CameraComponent();
			pScene->SetCamera(a);
			return a; }));
	
	
}

/*
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
*/
void ActorFactory::ModifyActor(Actor * pActor, tinyxml2::XMLElement * overrides)
{
}


ActorComponent * ActorFactory::VCreateComponent(const tinyxml2::XMLElement* pData)
{
	const char* name = pData->Value();
	auto factory = m_ComponentFactory.find(name);
	ActorComponent* pComponent = factory->second();
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

