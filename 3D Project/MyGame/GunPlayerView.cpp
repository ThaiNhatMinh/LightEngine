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

	return S_OK;
}


void GunPlayerView::Shoot()
{
	GetComponent<PVAnimationComponent>(PVAnimationComponent::Name)->PlayAnimation(m_AnimationMap[FIRE]);
	m_State = WS_SHOOTING;
}

void GunPlayerView::Reload()
{
	GetComponent<PVAnimationComponent>(PVAnimationComponent::Name)->PlayAnimation(m_AnimationMap[RELOAD]);
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
