
#include <pch.h>
#include "..\Interface\IFactory.h"
#include "..\Interface\IEventManager.h"
#include "..\Core\Events.h"
#include "Scene.h"

namespace Light
{
	Scene::Scene(IContext* c,const std::string& file) :m_Context(c)
	{
		m_pRoot = std::unique_ptr<IActor>(m_Context->GetSystem<IFactory>()->VCreateActor(SYSTEM_ROOT_ACTOR));
		if (!m_pRoot)
		{
			E_ERROR("Can't create Root Node.");
		}
	
		this->Load(file);
	}

	Scene::~Scene()
	{
	}

	
	bool Scene::Load(const std::string& filename)
	{
		
		tinyxml2::XMLDocument doc;
		if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS)
		{
			E_ERROR("Can't load Scene file: %s", filename.c_str());
			return 0;
		}

		tinyxml2::XMLElement* pScene = doc.FirstChildElement("Scene");
		if (!pScene)
		{
			E_ERROR("Can't load Scene file: %s", filename.c_str());
			return 0;
		}

		m_Name = pScene->Attribute("name");

		for (tinyxml2::XMLElement* pNode = pScene->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			if (!strcmp(pNode->Value(), "Actor"))
			{
				const char* pFile = pNode->Attribute("File");
				if (!pFile) continue;
				IActor* p4 = m_Context->GetSystem<IFactory>()->VCreateActor(pFile);
				m_pRoot->VAddChild(p4);
			}

		}


		m_pRoot->PostInit();

		
		tinyxml2::XMLElement* pLightNode = pScene->FirstChildElement("Light");
		
		m_LightManager.VSerialize(m_Context, pLightNode);
		
		auto pSkybox = pScene->FirstChildElement("SkyBox");

		m_SkyBox.VSerialize(m_Context, pSkybox);

		m_Context->GetSystem<IEventManager>()->VQueueEvent(std::shared_ptr<IEvent>(DEBUG_NEW events::EvtSceneCreate(this)));
		return 1;
	}

	bool Scene::VOnRender()
	{
		m_SkyBox.VRender();
		return true;
	}

	bool Scene::VOnUpdate(float dt)
	{
		m_pRoot->VOnUpdate(this, dt);
		//m_Context->m_pDebuger->Update();
		return true;
	}
	bool Scene::VPostUpdate()
	{
		m_pRoot->VPostUpdate(this);
		return true;
	}
	std::string Scene::VGetSceneName()
	{
		return m_Name;
	}
	render::SkyBox * Scene::GetSkyBox()
	{
		return &m_SkyBox;
	}
	render::LightManager * Scene::GetLightManager()
	{
		return &m_LightManager;
	}
}