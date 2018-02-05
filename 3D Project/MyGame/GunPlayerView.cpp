#include "pch.h"

GunPlayerView::GunPlayerView(ActorId id):PlayerView(id)
{
	m_State = WS_IDLE;
}

GunPlayerView::~GunPlayerView()
{
}

HRESULT GunPlayerView::VOnUpdate(Scene * pScene, float elapsedMs)
{
	Actor::VOnUpdate(pScene, elapsedMs);

	if (m_State == WS_RELOADING)
	{
		if (GetComponent<PVAnimationComponent>(PVAnimationComponent::Name)->IsFinish()) 
			m_State = WS_IDLE;
	}
	else if (m_State == WS_SHOOTING)
	{
		if (GetComponent<PVAnimationComponent>(PVAnimationComponent::Name)->IsFinish())
		{
			m_State = WS_IDLE;
			this->ShootRayCast();
		}
	}
	else if (m_State == WS_SELECT)
	{
		if (GetComponent<PVAnimationComponent>(PVAnimationComponent::Name)->IsFinish()) m_State = WS_IDLE;
	}

	return S_OK;
}


void GunPlayerView::Shoot()
{
	GetComponent<PVAnimationComponent>(PVAnimationComponent::Name)->PlayAnimation(m_AnimationMap[FIRE]);
	if(m_State == WS_IDLE)GetComponent<SoundSource3D>(SoundSource3D::Name)->Play("ShootAK47");
	m_State = WS_SHOOTING;
}

void GunPlayerView::Reload()
{
	GetComponent<PVAnimationComponent>(PVAnimationComponent::Name)->PlayAnimation(m_AnimationMap[RELOAD]);
	GetComponent<SoundSource3D>(SoundSource3D::Name)->Play("ReloadAK47");
	m_State = WS_RELOADING;
}

void GunPlayerView::Select()
{
	GetComponent<PVAnimationComponent>(PVAnimationComponent::Name)->PlayAnimation(m_AnimationMap[SELECT]);
	m_State = WS_SELECT;
}

void GunPlayerView::Run()
{
	if(m_State != WS_SHOOTING && m_State !=WS_RELOADING)
		GetComponent<PVAnimationComponent>(PVAnimationComponent::Name)->PlayAnimation(m_AnimationMap[RUN]);
}

void GunPlayerView::SetData(ModelCache * pModel)
{
	auto PVRender = GetComponent<MeshRenderComponent>(MeshRenderComponent::Name);
	PVRender->SetData(pModel);
	auto PVAnimation = GetComponent<BaseAnimComponent>(PVAnimationComponent::Name);
	PVAnimation->SetData(pModel);

	for (size_t i=0; i<pModel->pAnims.size(); i++)
	{
		if (pModel->pAnims[i]->Name == "fire") m_AnimationMap.insert({ FIRE,i });
		else if(pModel->pAnims[i]->Name == "idle_0")m_AnimationMap.insert({ IDLE,i });
		else if (pModel->pAnims[i]->Name == "postfire")m_AnimationMap.insert({ POST_FIRE,i });
		else if (pModel->pAnims[i]->Name == "prefire")m_AnimationMap.insert({ PRE_FIRE,i });
		else if (pModel->pAnims[i]->Name == "reload")m_AnimationMap.insert({ RELOAD,i });
		else if (pModel->pAnims[i]->Name == "select")m_AnimationMap.insert({ SELECT,i });
		else if (pModel->pAnims[i]->Name == "run")m_AnimationMap.insert({ RUN,i });
		else m_AnimationMap.insert({ IDLE,i });
	}


}

void GunPlayerView::ShootRayCast()
{
	PhysicsRaycastResult raycast;
	ICamera* pCam = Camera::GetCurrentCamera();
	Ray r(pCam->GetPosition(), pCam->GetFront());

	m_Context->m_pPhysic->RayCast(raycast, r, 1000000.f, TYPE_HITBOX);


	if (raycast.position != vec3(0))
	{
		Creature* attacker = static_cast<Creature*>(m_pParent->VGetParent());
		Creature* victim = static_cast<Creature*>(raycast.body->GetOwner());
		std::shared_ptr<IEvent> TakeDamageEvent = std::shared_ptr<IEvent>(new EvtTakeDamage(attacker, victim, 100));
		m_Context->m_pEventManager->VQueueEvent(TakeDamageEvent);
		
	}
	else
	{
		//printf("Doesn't hit anything.\n");
	}
	
}
