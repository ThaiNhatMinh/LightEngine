#include "stdafx.h"

Zombie::Zombie(ActorId id):Creature(id), m_MeshRender(nullptr)
{
}

Zombie::~Zombie()
{
}

void Zombie::PostInit(void)
{
	Creature::PostInit();
	m_MeshRender = RemoveComponent<MeshRenderComponent>(MeshRenderComponent::Name);
}

bool Zombie::VIsVisible(Scene * pScene) const
{
	ICamera* pCam = pScene->GetCurrentCamera();
	auto box = GetComponent<AnimationComponent>(AnimationComponent::Name)->GetUserDimesion();
	box.Translate(m_TransformComponent->GetPosition());
	return pCam->GetFrustum()->Inside(box.Min, box.Max);
}

HRESULT Zombie::VRender(Scene * pScene)
{
	m_MeshRender->Render(pScene);
	return S_OK;
}