#include "pch.h"

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
	ICamera* pCam = Camera::GetCurrentCamera();
	auto box = GetComponent<AnimationComponent>(AnimationComponent::Name)->GetUserDimesion();
	box.Translate(m_TransformComponent->GetPosition());
	return pCam->GetFrustum()->Inside(box.Min, box.Max);
}

HRESULT Zombie::VRender(Scene * pScene)
{
	GetComponent<MeshRenderComponent>(MeshRenderComponent::Name)->Render(pScene);
	return S_OK;
}

void Zombie::Death()
{
	//cout << m_Name << " death.\n";
	if (m_pParent->VRemoveChild(m_id))
	{
		//cout << "Delete : " << " success.\n";
		//delete this;
	}
	else
	{
		cout << "Can't delete : " << m_Name << "\n";
	}
}