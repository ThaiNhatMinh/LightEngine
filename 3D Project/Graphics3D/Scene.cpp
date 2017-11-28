#include "pch.h"
#include "OpenGLRenderer.h"


Scene::Scene(Context* c) :m_Debug(this, c), m_ActorFactory(this), m_CurrentCamera(nullptr), m_Context(c)
{
	m_pRoot = std::unique_ptr<Actor>(m_ActorFactory.CreateActor("GameAssets\\ACTOR\\Root.xml",nullptr,nullptr));
	if (!m_pRoot)
	{
		E_ERROR("Can't create Root Node.");
	}
	m_DirectionLight.La = vec3(0.1, 0.1, 0.1);
	m_DirectionLight.Ld = vec3(0.5, 0.5, 0.5);
	m_DirectionLight.Ls = vec3(1.0f, 1.0f, 1.0f);
	m_DirectionLight.direction = glm::normalize(vec3(1, -1, 1));

	m_DefaultCamera = Camera(c,vec3(0, 0, 100), vec3(0), vec3(0, 1, 0), 45.0f, 4.0f / 3.0, 1.0f, 10000.0f);
}

Scene::~Scene()
{
}

bool Scene::OnRender()
{
	OpenGLRenderer* O = m_Context->m_pRenderer.get();
	// The render passes usually go like this 
	// 1. Static objects & terrain
	// 2. Actors (dynamic objects that can move)
	// 3. The Sky
	// 4. Anything with Alpha
	
	// Root doesn't have anything to render, so just render children
	//m_pRoot->VRender(this);
	O->Clear();
	//glPolygonMode(GL_FRONT, GL_LINE);
	m_pRoot->VRenderChildren(this);

	//gPhysic()->VRenderDiagnostics();
	//m_Debug.DrawLine(vec3(0,0,0), vec3(0, 1000, 0), vec3(1, 1, 1));
	//m_Debug.DrawLine(vec3(0), vec3(2,0,0), vec3(1.0f, 1.0f, 1.0f));
	//m_Debug.DrawLine(vec3(0), vec3(0, 2, 0), vec3(1.0f, 1.0f, 1.0f));
	//m_Debug.DrawLine(vec3(0), vec3(0, 0, 2), vec3(1.0f, 1.0f, 1.0f));
	//m_Debug.Render();
	m_Context->m_pConsole->Draw();
	O->SwapBuffer();
	return true;
}

bool Scene::OnUpdate(float dt)
{
	m_DefaultCamera.Update(dt);
	m_pRoot->VOnUpdate(this, dt);
	return true;
}
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
/*
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