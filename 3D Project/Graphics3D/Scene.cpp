#include "Scene.h"
#include "Scene.h"
#include "pch.h"
#include "OpenGLRenderer.h"


Scene::Scene(Context* c) :m_Context(c),m_CurrentCamera(nullptr)
{
	m_pRoot = std::unique_ptr<Actor>(m_Context->GetSystem<ActorFactory>()->CreateActor("GameAssets\\ACTOR\\Root.xml",nullptr,0));
	if (!m_pRoot)
	{
		E_ERROR("Can't create Root Node.");
	}
	m_DirectionLight.La = vec3(0.2, 0.2, 0.2);
	m_DirectionLight.Ld = vec3(0.5, 0.5, 0.5);
	m_DirectionLight.Ls = vec3(1.0f, 1.0f, 1.0f);
	m_DirectionLight.direction = glm::normalize(vec3(-1, -1,- 1));

	//m_DefaultCamera = Camera(c,vec3(0, 0, 100.0f), vec3(.0f), vec3(0, 1.0f, 0), 45.0f, 4.0f / 3.0f, 1.0f, 10000.0f);
}

Scene::~Scene()
{
}

bool Scene::LoadScene(const string& filename)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filename.c_str())!= tinyxml2::XML_SUCCESS)
	{
		E_ERROR("Can't load Scene file: " + filename);
		return 0;
	}

	tinyxml2::XMLElement* pScene = doc.FirstChildElement("Scene");
	if (!pScene)
	{
		E_ERROR("Can't load Scene file: " + filename);
		return 0;
	}

	for (tinyxml2::XMLElement* pNode = pScene->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		if (!strcmp(pNode->Value(), "Actor"))
		{
			const char* pFile = pNode->Attribute("File");
			if (!pFile) continue;
			Actor* p4 = m_Context->GetSystem<ActorFactory>()->CreateActor(pFile, nullptr, 0);
			m_pRoot->VAddChild(std::unique_ptr<Actor>(p4));
		}
	}

	return 1;
}

bool Scene::OnRender()
{
	
	// The render passes usually go like this 
	// 1. Static objects & terrain
	// 2. Actors (dynamic objects that can move)
	// 3. The Sky
	// 4. Anything with Alpha
	
	// Root doesn't have anything to render, so just render children	
	m_pRoot->VRenderChildren(this);
	
	// render last
	//m_Context->m_pRenderer->ClearBuffer();
	//ImGui::Text("Size: %d", m_ActorLast.size());
	while (m_ActorLast.size()>0)
	{
		Actor* pActor = m_ActorLast.back();
		m_ActorLast.pop_back();
		pActor->GetComponent<MeshRenderComponent>("MeshRenderComponent")->Render(this);
	}
	
	return true;
}

bool Scene::OnUpdate(float dt)
{
	m_pRoot->VOnUpdate(this, dt);
	//m_Context->m_pDebuger->Update();
	return true;
}
bool Scene::PostUpdate()
{
	m_pRoot->VPostUpdate(this);
	return true;
}

ICamera * Scene::GetCurrentCamera()
{
	return m_CurrentCamera;
}

void Scene::SetCurrentCamera(Camera * cam)
{
	m_CurrentCamera = cam;
}

void Scene::PushLastActor(Actor * pActor)
{
	m_ActorLast.push_back(pActor);
}
