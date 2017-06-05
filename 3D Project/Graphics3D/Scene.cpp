#include "..\pch.h"

Scene::Scene()
{
	m_pRoot = gActorFactory()->CreateActor("GameAssets\Root.xml",nullptr,nullptr);
}

Scene::~Scene()
{
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
	m_pRoot->VOnUpdate(this, dt);
	return true;
}

mat4 Scene::GetViewProj()
{
	mat4 view = m_Camera->GetViewMatrix();
	mat4 proj = m_Frustum->GetProjMatrix();
	mat4 viewproj = view*proj;
	return viewproj;
}
