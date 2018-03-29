#include "Scene.h"
#include "Scene.h"
#include <pch.h>
#include "OpenGLRenderer.h"


Scene::Scene(Context* c) :m_Context(c),m_CurrentCamera(nullptr)
{
	m_pRoot = std::unique_ptr<Actor>(m_Context->GetSystem<ActorFactory>()->CreateActor("GameAssets\\ACTOR\\Root.xml",nullptr,0));
	if (!m_pRoot)
	{
		E_ERROR("Can't create Root Node.");
	}
	m_DirectionLight.La = vec3(1.0f, 1.0f, 1.0f);
	m_DirectionLight.Ld = vec3(1.0f, 1.0f, 1.0f);
	m_DirectionLight.Ls = vec3(1.0f, 1.0f, 1.0f);
	m_DirectionLight.direction = glm::normalize(vec3(-1, -3,- 1));

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

	tinyxml2::XMLElement* pLightNode = doc.FirstChildElement("Light");
	if (pLightNode)
	{
		tinyxml2::XMLElement* pDirLight = pLightNode->FirstChildElement("Direction");
		if (pDirLight)
		{
			m_DirectionLight.direction.x = pDirLight->DoubleAttribute("x");
			m_DirectionLight.direction.y = pDirLight->DoubleAttribute("y");
			m_DirectionLight.direction.z = pDirLight->DoubleAttribute("z");
			m_DirectionLight.direction = glm::normalize(m_DirectionLight.direction);

			tinyxml2::XMLElement* pAmbient = pDirLight->FirstChildElement("Ambient");
			m_DirectionLight.La.x = pAmbient->DoubleAttribute("r",1.0f);
			m_DirectionLight.La.y = pAmbient->DoubleAttribute("g", 1.0f);
			m_DirectionLight.La.z = pAmbient->DoubleAttribute("b", 1.0f);

			tinyxml2::XMLElement* pDiffuse = pDirLight->FirstChildElement("Diffuse");
			m_DirectionLight.Ld.x = pDiffuse->DoubleAttribute("r", 1.0f);
			m_DirectionLight.Ld.y = pDiffuse->DoubleAttribute("g", 1.0f);
			m_DirectionLight.Ld.z = pDiffuse->DoubleAttribute("b", 1.0f);

			tinyxml2::XMLElement* pSpecular = pDirLight->FirstChildElement("Specular");
			m_DirectionLight.Ls.x = pSpecular->DoubleAttribute("r", 1.0f);
			m_DirectionLight.Ls.y = pSpecular->DoubleAttribute("g", 1.0f);
			m_DirectionLight.Ls.z = pSpecular->DoubleAttribute("b", 1.0f);
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
	
	
	//m_pSkyBox->VRender(this);
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
