#include "..\pch.h"

Scene::Scene()
{
	m_pRoot = gActorFactory()->CreateActor("GameAssets\\Root.xml",nullptr,&mat4());
	if (!m_pRoot)
	{
		E_ERROR("Can't create Root Node.");
	}
	m_DirectionLight.La = vec3(0.1, 0.1, 0.1);
	m_DirectionLight.Ld = vec3(0.5, 0.5, 0.5);
	m_DirectionLight.Ls = vec3(1.0f, 1.0f, 1.0f);
	m_DirectionLight.direction = glm::normalize(vec3(1, -1, 1));
}

Scene::~Scene()
{
	delete m_pRoot;
	delete m_Camera;
	delete m_Frustum;
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
	m_pRoot->VRenderChildren(this);
		
	return true;
}

bool Scene::OnUpdate(float dt)
{
	m_Camera->Update(dt);
	m_Frustum->Update(*m_Camera);

	m_pRoot->VOnUpdate(this, dt);
	return true;
}

mat4 Scene::GetViewProj()
{
	mat4 view = m_Camera->GetViewMatrix();
	mat4 proj = m_Frustum->GetProjMatrix();
	mat4 viewproj = proj*view;
	return viewproj;
}
