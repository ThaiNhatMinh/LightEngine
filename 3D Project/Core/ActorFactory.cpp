#include <pch.h>
#include "ActorFactory.h"
#include "Events.h"
#include "..\Interface\IEvent.h"
#include "..\Graphics3D\Actor.h"
#include "..\GameComponents\TransformComponent.h"
#include "..\GameComponents\MeshRenderComponent.h"
#include "..\Graphics3D\DefaultMaterial.h"
#include "..\Graphics3D\Scene.h"
namespace Light
{

	ActorFactory::ActorFactory(IContext* c):m_pContext(c)
	{
		m_lastActorId = 1;
		m_ComponentFactoryMap.insert(std::make_pair("TransformComponent", []() { return DEBUG_NEW TransformComponent(); }));
		m_ComponentFactoryMap.insert(std::make_pair("MeshRenderComponent", []() { return DEBUG_NEW MeshRenderComponent(); }));
		/*m_ComponentFactoryMap.insert(std::make_pair("ColliderComponent", []() { return DEBUG_NEW ColliderComponent(); }));
		m_ComponentFactoryMap.insert(std::make_pair("RigidBodyComponent", []() { return DEBUG_NEW RigidBodyComponent(); }));
		m_ComponentFactoryMap.insert(std::make_pair("AnimationComponent", []() { return DEBUG_NEW AnimationComponent(); }));
		m_ComponentFactoryMap.insert(std::make_pair("PVAnimationComponen", []() { return DEBUG_NEW PVAnimationComponent(); }));
		m_ComponentFactoryMap.insert(std::make_pair("CharacterControllerComponent", []() { return DEBUG_NEW CharacterControllerComponent(); }));
		m_ComponentFactoryMap.insert(std::make_pair("LogicComponent", []() { return DEBUG_NEW LogicComponent(); }));
		
		m_ComponentFactoryMap.insert(std::make_pair("TerrainRenderComponent", []() { return DEBUG_NEW TerrainRenderComponent(); }));
		m_ComponentFactoryMap.insert(std::make_pair("CameraComponent", []() { return  DEBUG_NEW CameraComponent(); }));
		m_ComponentFactoryMap.insert(std::make_pair("HitBox", []() { return  DEBUG_NEW HitBox(); }));
		m_ComponentFactoryMap.insert(std::make_pair("SoundListener", []() { return  DEBUG_NEW SoundListener(); }));
		m_ComponentFactoryMap.insert(std::make_pair("SoundSource3D", []() { return  DEBUG_NEW SoundSource3D(); }));


		m_ActorFactoryMap.insert(std::make_pair("Actor", [](int id) {return DEBUG_NEW Actor(id); }));
		m_ActorFactoryMap.insert(std::make_pair("World", [](int id) {return DEBUG_NEW TerrainWorld(id); }));
		m_ActorFactoryMap.insert(std::make_pair("StaticObject", [](int id) {return DEBUG_NEW StaticObject(id); }));
		m_ActorFactoryMap.insert(std::make_pair("SkyBox", [](int id) {return DEBUG_NEW SkyBox(id); }));*/


		/*m_ShaderFactory.insert(std::make_pair("SkeShader", [](const char*vs, const char* fs) {return DEBUG_NEW SkeShader(vs, fs); }));
		m_ShaderFactory.insert(std::make_pair("PrimShader", [](const char*vs, const char* fs) {return DEBUG_NEW PrimShader(vs, fs); }));
		m_ShaderFactory.insert(std::make_pair("Debug", [](const char*vs, const char* fs) {return DEBUG_NEW DebugShader(vs, fs); }));
		m_ShaderFactory.insert(std::make_pair("Shader", [](const char*vs, const char* fs) {return DEBUG_NEW Shader(vs, fs); }));
		m_ShaderFactory.insert(std::make_pair("ImGuiShader", [](const char*vs, const char* fs) {return DEBUG_NEW ImGuiShader(vs, fs); }));
		m_ShaderFactory.insert(std::make_pair("SpriteShader", [](const char*vs, const char* fs) {return DEBUG_NEW SpriteShader(vs, fs); }));*/

		m_MaterialMap.insert(std::make_pair("Default", std::shared_ptr<render::Material>(DEBUG_NEW render::DefaultMaterial(m_pContext))));
		c->VAddSystem(this);
		m_pEventManager = c->GetSystem<IEventManager>();
		
	}

	ActorFactory::~ActorFactory()
	{

	}



	IComponent * ActorFactory::CreateComponent(const tinyxml2::XMLElement* pData)
	{
		const char* name = pData->Value();
		IComponent* pComponent;
		auto factory = m_ComponentFactoryMap.find(name);
		if (factory != m_ComponentFactoryMap.end()) pComponent = factory->second();
		else
		{
			E_ERROR("ActorFactory::CreateComponent() can't find: %s",name);
			return nullptr;
		}

		return pComponent;
	}


	bool ActorFactory::VRegisterComponentFactory(const std::string& name, std::function<IComponent*()>func)
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
			E_ERROR("Failed to create Actor from file: %s", actorResource);
			return nullptr;
		}

		tinyxml2::XMLElement* pActorData = doc.FirstChildElement("Actor");

		const char* type = pActorData->Attribute("type");
		auto factory = m_ActorFactoryMap.find(type);
		IActor* pActor = nullptr;
		if (factory != m_ActorFactoryMap.end()) pActor =factory->second(GetNextActorId());
		else pActor = DEBUG_NEW Actor(GetNextActorId());


		if (!pActor->Init(pActorData))
		{
			E_ERROR("Failed to init Actor:" ,actorResource);
			return nullptr;
		}

		tinyxml2::XMLElement* pComponentData = pActorData->FirstChildElement("Component");
		// Loop through each child element and load the component
		for (tinyxml2::XMLElement* pNode = pComponentData->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			IComponent* pComponent(CreateComponent(pNode));
			if (pComponent)
			{
				pComponent->SetOwner(pActor);
				if (!pComponent->VSerialize(m_pContext,pNode))
				{
					E_WARNING("Actor: %s, Component failed to initialize: %s", pActor->VGetName().c_str(),pNode->Value());
					continue;
				}
				pActor->VAddComponent(pComponent);

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

		if (!isCreateChild) pActor->PostInit();

		if (!m_pEventManager->VQueueEvent(std::shared_ptr<IEvent>(DEBUG_NEW events::EvtNewActor(pActor))))
		{
			E_ERROR("Failer to send event");
		}
		return pActor;
	}

	bool ActorFactory::VRegisterMaterial(const std::string & name, std::function<render::Material*()> func)
	{
		auto result = m_MaterialMap.find(name);
		if (result != m_MaterialMap.end())
		{
			E_ERROR("Material: %s has already exits!", name.c_str());
			return false;
		}

		m_MaterialMap.insert(std::pair<std::string, render::Material*>(name, func()));

		return true;
	}

	std::shared_ptr<render::Material> ActorFactory::VGetMaterial(const std::string & name)
	{
		auto result = m_MaterialMap.find(name);
		if(result==m_MaterialMap.end()) return nullptr;

		return result->second;
	}

	const char * ActorFactory::VGetName()
	{
		return typeid(IFactory).name();
	}

	IScene * ActorFactory::VCreateScene(const std::string & name)
	{
		auto pScene = DEBUG_NEW Scene(m_pContext,name);
		m_Scenes.push_back(std::unique_ptr<IScene>(pScene));

		return pScene;
	}

	/*Shader * ActorFactory::VCreateShader(const char * type, const char * vs, const char * fs)
	{
		auto func = m_ShaderFactory.find(type);
		if (func == m_ShaderFactory.end()) return nullptr;

		Shader* p = func->second(vs, fs);
		return p;
	}*/

}
