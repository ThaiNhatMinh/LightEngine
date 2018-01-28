#include "pch.h"
#include "OpenGLRenderer.h"


Scene::Scene(Context* c) :m_Context(c)
{
	m_pRoot = std::unique_ptr<Actor>(m_Context->m_pActorFactory->CreateActor("GameAssets\\ACTOR\\Root.xml",nullptr,0));
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
			Actor* p4 = m_Context->m_pActorFactory->CreateActor(pFile, nullptr, 0);
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

	//gPhysic()->VRenderDiagnostics();
	//m_Debug.DrawLine(vec3(0,0,0), vec3(0, 1000, 0), vec3(1, 1, 1));
	//m_Debug.DrawLine(vec3(0), vec3(2,0,0), vec3(1.0f, 1.0f, 1.0f));
	//m_Debug.DrawLine(vec3(0), vec3(0, 2, 0), vec3(1.0f, 1.0f, 1.0f));
	//m_Debug.DrawLine(vec3(0), vec3(0, 0, 2), vec3(1.0f, 1.0f, 1.0f));
	//m_Debug.Render();
	
	
	return true;
}

bool Scene::OnUpdate(float dt)
{
	//m_CurrentCamera = CameraComponent::m_CameraList.back();
	//m_DefaultCamera.Update(dt);
	m_pRoot->VOnUpdate(this, dt);
	m_Context->m_pDebuger->Update();
	return true;
}
bool Scene::PostUpdate()
{
	m_pRoot->VPostUpdate(this);
	return true;
}
/*
void Scene::SetCamera(CameraComponent * pCam)
{
	m_CurrentCamera = pCam;
}
mat4 Scene::GetViewProj()
{
	if(m_CurrentCamera)
	return m_CurrentCamera->GetVPMatrix();
	else 
		return m_DefaultCamera.GetVPMatrix();
}

void Scene::SetCameraNode(Actor * pActor)
{
	m_pCameraNode = pActor->GetComponent<CameraComponent>(CameraComponent::Name);
}

mat4 Scene::GetViewProj()
{
	mat4 view = m_Camera->GetViewMatrix();
	mat4 proj = m_Frustum->GetProjMatrix();
	mat4 viewproj = proj*view;
	return viewproj;
}
*/