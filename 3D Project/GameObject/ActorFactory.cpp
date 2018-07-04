#include <pch.h>


ActorFactory::ActorFactory(Context* c)
{
	m_lastActorId = 1;
	m_ComponentFactoryMap.insert(std::make_pair("TransformComponent", []() { return new TransformComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair("ColliderComponent", []() { return new ColliderComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair("RigidBodyComponent", []() { return new RigidBodyComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair("AnimationComponent", []() { return new AnimationComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair("PVAnimationComponen", []() { return new PVAnimationComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair("CharacterControllerComponent", []() { return new CharacterControllerComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair("LogicComponent", []() { return new LogicComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair("MeshRenderComponent", []() { return new MeshRenderComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair("TerrainRenderComponent", []() { return new TerrainRenderComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair("CameraComponent", [](){ return  new CameraComponent(); }));
	m_ComponentFactoryMap.insert(std::make_pair("HitBox", []() { return  new HitBox(); }));
	m_ComponentFactoryMap.insert(std::make_pair("SoundListener", []() { return  new SoundListener(); }));
	m_ComponentFactoryMap.insert(std::make_pair("SoundSource3D", []() { return  new SoundSource3D(); }));
	
	
	m_ActorFactoryMap.insert(std::make_pair("Actor", [](int id) {return new Actor(id); }));
	m_ActorFactoryMap.insert(std::make_pair("World", [](int id) {return new TerrainWorld(id); }));
	m_ActorFactoryMap.insert(std::make_pair("StaticObject", [](int id) {return new StaticObject(id); }));
	m_ActorFactoryMap.insert(std::make_pair("SkyBox", [](int id) {return new SkyBox(id); }));
	

	m_ShaderFactory.insert(std::make_pair("SkeShader", [](const char*vs, const char* fs) {return new SkeShader(vs, fs); }));
	m_ShaderFactory.insert(std::make_pair("PrimShader", [](const char*vs, const char* fs) {return new PrimShader(vs, fs); }));
	m_ShaderFactory.insert(std::make_pair("Debug", [](const char*vs, const char* fs) {return new DebugShader(vs, fs); }));
	m_ShaderFactory.insert(std::make_pair("Shader", [](const char*vs, const char* fs) {return new Shader(vs, fs); }));
	m_ShaderFactory.insert(std::make_pair("ImGuiShader", [](const char*vs, const char* fs) {return new ImGuiShader(vs, fs); }));
	m_ShaderFactory.insert(std::make_pair("SpriteShader", [](const char*vs, const char* fs) {return new SpriteShader(vs, fs); }));

	
	c->AddSystem(this);
	m_pEventManager = c->GetSystem<EventManager>();
}

ActorFactory::~ActorFactory()
{
	
}



ActorComponent * ActorFactory::CreateComponent(const tinyxml2::XMLElement* pData)
{
	const char* name = pData->Value();
	ActorComponent* pComponent;
	auto factory = m_ComponentFactoryMap.find(name);
	if(factory!= m_ComponentFactoryMap.end()) pComponent = factory->second();
	else
	{
		E_ERROR("ActorFactory::VCreateComponent() can't find: " + string(name));
		return nullptr;
	}
	
	return pComponent;
}


bool ActorFactory::VRegisterComponentFactory(string name, std::function<ActorComponent*()>func)
{
	auto it = m_ComponentFactoryMap.find(name);
	if (it != m_ComponentFactoryMap.end())
	{
		m_ComponentFactoryMap.erase(name);
	}

	m_ComponentFactoryMap.insert(std::make_pair(name, func));
	return 1;
}

bool ActorFactory::VRegisterActorFactory(const string & name, std::function<IActor*(int id)> func)
{
	auto it = m_ActorFactoryMap.find(name);
	if (it != m_ActorFactoryMap.end()) return false;

	m_ActorFactoryMap.insert(std::make_pair(name, func));
	return true;
}

IActor * ActorFactory::VCreateActor(const char* actorResource, bool isCreateChild)
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
	if(factory != m_ActorFactoryMap.end()) pActor = static_cast<Actor*>(factory->second(GetNextActorId()));
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
		ActorComponent* pComponent(CreateComponent(pNode));
		if (pComponent)
		{
			pComponent->SetOwner(pActor);
			if (!pComponent->VInit(pNode))
			{
				E_WARNING(pActor->VGetName() + ": Component failed to initialize: " + std::string(pNode->Value()));
				continue;
			}
			if (!strcmp(pNode->Value(), "TransformComponent")) pActor->SetTransformComponent(static_cast<TransformComponent*>(pComponent));
			else pActor->AddComponent(pComponent);

		}
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
			IActor* child = VCreateActor(pFile, 1);
			pActor->VAddChild(child);
			child->PostInit();
		}

	}
	
	if(!isCreateChild) pActor->PostInit();

	if (m_pEventManager->VQueueEvent(std::shared_ptr<IEvent>(new EvtData_New_Actor(pActor))))
	{
		E_ERROR("Failer to send event");
	}
	return pActor;
}

Shader * ActorFactory::VCreateShader(const char * type, const char * vs, const char * fs)
{
	auto func = m_ShaderFactory.find(type);
	if (func == m_ShaderFactory.end()) return nullptr;

	Shader* p = func->second(vs, fs);
	return p;
}
