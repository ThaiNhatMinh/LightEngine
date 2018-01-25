#include "pch.h"

void ActorFactory::Init(Context * c)
{
	c->m_pActorFactory = std::unique_ptr<ActorFactory>(this);
	
}

void ActorFactory::ShutDown()
{
	
}

ActorFactory::ActorFactory()
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
	m_ComponentFactoryMap.insert(std::make_pair(CameraComponent::Name, [](){ return  new CameraComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair(HitBox::Name, []() { return  new HitBox(); }));
	
	m_ActorFactoryMap.insert(std::make_pair("Player", [](int id) {return new Player(id); }));
	m_ActorFactoryMap.insert(std::make_pair("World", [](int id) {return new TerrainWorld(id); }));
	m_ActorFactoryMap.insert(std::make_pair("PlayerView", [](int id) {return new PlayerView(id); }));
	m_ActorFactoryMap.insert(std::make_pair("Weapon", [](int id) {return new Weapon(id); }));
	m_ActorFactoryMap.insert(std::make_pair("StaticObject", [](int id) {return new StaticObject(id); }));
	

	m_ShaderFactory.insert(std::make_pair("SkeShader", [](const char*vs, const char* fs) {return new SkeShader(vs, fs); }));
	m_ShaderFactory.insert(std::make_pair("PrimShader", [](const char*vs, const char* fs) {return new PrimShader(vs, fs); }));
	m_ShaderFactory.insert(std::make_pair("Debug", [](const char*vs, const char* fs) {return new DebugShader(vs, fs); }));
	m_ShaderFactory.insert(std::make_pair("Shader", [](const char*vs, const char* fs) {return new Shader(vs, fs); }));
	m_ShaderFactory.insert(std::make_pair("ImGuiShader", [](const char*vs, const char* fs) {return new ImGuiShader(vs, fs); }));
	m_ShaderFactory.insert(std::make_pair("SpriteShader", [](const char*vs, const char* fs) {return new SpriteShader(vs, fs); }));
	
	
}

ActorFactory::~ActorFactory()
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


bool ActorFactory::RegisterComponentFactory(string name, std::function<ActorComponent*()>func)
{
	auto it = m_ComponentFactoryMap.find(name);
	if (it != m_ComponentFactoryMap.end()) return false;

	m_ComponentFactoryMap.insert(std::make_pair(name, func));
	return 1;
}

Actor * ActorFactory::CreateActor(const char* actorResource, const mat4* initialTransform, bool isCreateChild)
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
				E_ERROR(pActor->VGetName() + ": Component failed to initialize: " + std::string(pNode->Value()));
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


	if (initialTransform)
	{
		TransformComponent* pTc = pActor->GetTransform();
		pTc->SetTransform(*initialTransform);
	}

	

	// load child
	tinyxml2::XMLElement* pChildData = pActorData->FirstChildElement("Children");

	if (pChildData)
	{

		for (tinyxml2::XMLElement* pNode = pChildData->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			const char* pFile = pNode->Attribute("File");
			if (!pFile) continue;
			if (strlen(pFile) < 1) continue;
			Actor* child = CreateActor(pFile, nullptr, 1);
			pActor->VAddChild(std::unique_ptr<Actor>(child));
			child->PostInit();
		}

	}
	
	if(!isCreateChild) pActor->PostInit();

	return pActor;
}

Shader * ActorFactory::CreateShader(const char * type, const char * vs, const char * fs)
{
	auto func = m_ShaderFactory.find(type);
	if (func == m_ShaderFactory.end()) return nullptr;

	Shader* p = func->second(vs, fs);
	return p;
}
