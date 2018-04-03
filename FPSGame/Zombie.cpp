#include "stdafx.h"

Zombie::Zombie(ActorId id):Creature(id)
{
}

Zombie::~Zombie()
{
}

void Zombie::PostInit(void)
{
	Creature::PostInit();
	
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
	GetComponent<MeshRenderComponent>(MeshRenderComponent::Name)->Render(pScene);
	return S_OK;
}