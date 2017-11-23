#include "pch.h"
#include "OpenGLRenderer.h"


Scene::Scene(RenderAPICore* pRender):m_Debug(this), m_ActorFactory(this), m_CurrentCamera(nullptr)
{
	m_pRoot = m_ActorFactory.CreateActor("GameAssets\\Root.xml",nullptr,&mat4());
	if (!m_pRoot)
	{
		E_ERROR("Can't create Root Node.");
	}
	m_DirectionLight.La = vec3(0.1, 0.1, 0.1);
	m_DirectionLight.Ld = vec3(0.5, 0.5, 0.5);
	m_DirectionLight.Ls = vec3(1.0f, 1.0f, 1.0f);
	m_DirectionLight.direction = glm::normalize(vec3(1, -1, 1));

	m_pRenderer = pRender;
	m_DefaultCamera = Camera(vec3(0, 0, 100), vec3(0), vec3(0, 1, 0), 45.0f, 4.0f / 3.0, 1.0, 10000.0f);
}

Scene::~Scene()
{
	m_pRenderer->ShutDown();
	delete m_pRenderer;
	delete m_pRoot;
}

bool Scene::OnRender()
{
	// The render passes usually go like this 
	// 1. Static objects & terrain
	// 2. Actors (dynamic objects that can move)
	// 3. The Sky
	// 4. Anything with Alpha
	
	// Root doesn't have anything to render, so just render children
	//m_pRoot->VRender(this);
	m_pRenderer->Clear();
	//glPolygonMode(GL_FRONT, GL_LINE);
	m_pRoot->VRenderChildren(this);

	//gPhysic()->VRenderDiagnostics();
	m_Debug.DrawLine(vec3(0,0,0), vec3(1000, 1000, 1000), vec3(1, 1, 1));
	m_Debug.Render();

	m_pRenderer->SwapBuffer();
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