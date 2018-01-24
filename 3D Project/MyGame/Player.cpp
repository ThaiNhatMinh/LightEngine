#include <pch.h>
#include "Player.h"
#include <string.h>

Player::Player(ActorId id) :Creature(id)
{

}

Player::~Player()
{

}

bool Player::Init(const tinyxml2::XMLElement * pData)
{


	const tinyxml2::XMLElement * pInfo = pData->FirstChildElement("Info");

	int t = pInfo->DoubleAttribute("Team", 0);
	if (t == TEAM_BL) m_Team = TEAM_BL;
	else if (t == TEAM_GR) m_Team = TEAM_GR;

	const tinyxml2::XMLElement * pCharacter = pData->FirstChildElement("Character");

	m_Character = pCharacter->Attribute("Name");

	return Creature::Init(pData);
}

void Player::PostInit(void)
{
	Actor::PostInit();

	if (GetComponent<LocalPlayerComponent>(LocalPlayerComponent::Name))
	{
		
	}
}

bool Player::VIsVisible(Scene * pScene) const
{
	CameraComponent* cam = pScene->GetCamera();
	auto box =  GetComponent<AnimationComponent>(AnimationComponent::Name)->GetUserDimesion();
	box.Translate(m_TransformComponent->GetPosition());
	return cam->GetFrustum().Inside(box.Min, box.Max);
}

HRESULT Player::VRender(Scene * pScene)
{
	GetComponent<MeshRenderComponent>(MeshRenderComponent::Name)->Render(pScene);
	return S_OK;
}
vector<LTBSocket>& Player::GetSockets()
{
	return GetComponent<MeshRenderComponent>(MeshRenderComponent::Name)->GetSockets();
}
void Player::AddWeapon(Weapon * wp)
{
	assert(0);
}
/*
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
*/