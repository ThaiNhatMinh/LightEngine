#include <pch.h>
#include "Player.h"
#include <string.h>

Player::Player(ActorId id):Actor(id)
{
	
	m_Context->m_pEventManager->VAddListener(MakeDelegate(this,&Player::EventWeaponData), EvtData_PlayerWpData::sk_EventType);
	m_Context->m_pEventManager->VAddListener(MakeDelegate(this, &Player::EventCharacterData), EvtData_PlayerCharData::sk_EventType);
}

Player::~Player()
{
	
	m_Context->m_pEventManager->VRemoveListener(MakeDelegate(this, &Player::EventCharacterData), EvtData_PlayerCharData::sk_EventType);
	m_Context->m_pEventManager->VRemoveListener(MakeDelegate(this, &Player::EventWeaponData), EvtData_PlayerWpData::sk_EventType);
}

bool Player::Init(const tinyxml2::XMLElement * pData)
{
	Actor::Init(pData);

	const tinyxml2::XMLElement * pInfo = pData->FirstChildElement("Info");
	m_HP = pInfo->DoubleAttribute("HP", 100.0);
	m_AC = pInfo->DoubleAttribute("AC", 0);
	int t = pInfo->DoubleAttribute("Team", 0);
	if (t == TEAM_BL) m_Team = TEAM_BL;
	else if (t == TEAM_GR) m_Team = TEAM_GR;
	const tinyxml2::XMLElement * pWeapon = pData->FirstChildElement("Weapon");
	m_iCurrentWP = pWeapon->DoubleAttribute("Default", -1.0f);
	int i = 1;
	while (1)
	{
		char t[2];
		t[0] = '0' + i;
		t[1] = '\0';
		string name = "Name" + string(t);
		const char* Name = pWeapon->Attribute(name.c_str());
		if (!Name) break;
		EvtData_RequestCreateWeapon* p = new EvtData_RequestCreateWeapon(this, Name);
		p->File = "GameAssets\\ACTOR\\Weapon.xml";
		std::shared_ptr<const IEvent> pEvent(p);
		m_Context->m_pEventManager->VTriggerEvent(pEvent);
		i++;
	}

	const tinyxml2::XMLElement * pCharacter = pData->FirstChildElement("Character");

	m_Character = pCharacter->Attribute("Name");

	return 1;
}

void Player::PostInit(void)
{
	Actor::PostInit();

	m_MeshRender = std::unique_ptr<MeshRenderComponent>(RemoveComponent<MeshRenderComponent>(MeshRenderComponent::Name));
	m_AnimC = GetComponent<BaseAnimComponent>(AnimationComponent::Name);
}

bool Player::VIsVisible(Scene * pScene) const
{
	CameraComponent* cam = pScene->GetCamera();
	AABB box = m_AnimC->GetUserDimesion();
	box.Translate(m_TransformComponent->GetPosition());
	return cam->GetFrustum().Inside(box.Min, box.Max);
}

HRESULT Player::VRender(Scene * pScene)
{
	m_MeshRender->Render(pScene);
	return S_OK;
}

void Player::EventWeaponData(std::shared_ptr<const IEvent> pEvents)
{
	const EvtData_PlayerWpData *p = static_cast<const EvtData_PlayerWpData*>(pEvents.get());
	AnimationComponent* pAC = GetComponent<AnimationComponent>(AnimationComponent::Name);
	
	const vector<WeaponResource>& wr = p->wr;
	for (int i = 0; i < m_Children.size(); i++)
	{
		if (m_Children[i]->VGetTag() == "Weapon")
		{
			Weapon* pWp = static_cast<Weapon*>(m_Children[i].get());

			int j = 0;
			for (j = 0; j < wr.size(); j++)
				if (wr[j].Name == pWp->VGetName()) break;

			size_t i = 0;
			for (i = 0; i < m_RModel->Sockets.size(); i++)
				if (strstr(m_RModel->Sockets[i].m_Name, "weapo") != 0) 
					break;

			if (wr[j].TargetSlot == m_iCurrentWP) pAC->SetBaseAnim(wr[j].AnimName);

			m_WPList[wr[j].TargetSlot] = i;

			if(i== m_RModel->Sockets.size()) pWp->LoadData(wr[j], nullptr);
			else pWp->LoadData(wr[j], &m_RModel->Sockets[i]);
			
			
		}
	}
	
}

void Player::EventCharacterData(std::shared_ptr<const IEvent> pEvents)
{
	const EvtData_PlayerCharData *p = static_cast<const EvtData_PlayerCharData*>(pEvents.get());

	const vector<CharacterResource>& wr = p->wr;

	size_t i;
	for (i = 0; i < wr.size(); i++)
		if (wr[i].Name == m_Character) break;

	
	
	if (m_MeshRender == nullptr) return;

	m_RModel= m_Context->m_pResources->GetModel(wr[i].ModelFile[m_Team].c_str());
	if (m_RModel)
	{
		// load texture
		
		vector<std::unique_ptr<SkeMesh>>& ve = m_RModel->pMeshs;
		for (size_t j = 0; j < ve.size(); j++)
		{
			map<string, string> ss = wr[i].TexFile[m_Team];
			const char* pTextureFile = ss[ve[j]->Name].c_str();
			
			ve[j]->Tex = m_Context->m_pResources->GetTexture(pTextureFile);
			m_MeshRender->GetMeshList().push_back(m_RModel->pMeshs[j].get());
		}
	}
}
