#include <pch.h>
#include "Player.h"
#include <string.h>

Player::Player(ActorId id) :Creature(id), PlayerView(nullptr)
{

}

Player::~Player()
{

}

bool Player::Init(const tinyxml2::XMLElement * pData)
{


	const tinyxml2::XMLElement * pInfo = pData->FirstChildElement("Info");

	int t = pInfo->DoubleAttribute("Team", 0);
	m_iCurrentWP = pInfo->Int64Attribute("WP", -1);
	if (t == TEAM_BL) m_Team = TEAM_BL;
	else if (t == TEAM_GR) m_Team = TEAM_GR;

	const tinyxml2::XMLElement * pCharacter = pData->FirstChildElement("Character");

	m_Character = pCharacter->Attribute("Data");

	Mode = 0;
	return Creature::Init(pData);
}

void Player::PostInit(void)
{

	if (GetComponent<LocalPlayerComponent>(LocalPlayerComponent::Name))
	{
		SetPVModel();

	}
	Actor::PostInit();
	
}

bool Player::VIsVisible(Scene * pScene) const
{
	ICamera* pCam = Camera::GetCurrentCamera();
	auto box =  GetComponent<AnimationComponent>(AnimationComponent::Name)->GetUserDimesion();
	box.Translate(m_TransformComponent->GetPosition());
	return pCam->GetFrustum()->Inside(box.Min, box.Max);
}

HRESULT Player::VRender(Scene * pScene)
{
	if (!Mode)
	{
		static MeshRenderComponent* MeshRender = GetComponent<MeshRenderComponent>(MeshRenderComponent::Name);
		MeshRender->Render(pScene);
	}
	else
	{
		if(PlayerView) PlayerView->VRenderChildren(pScene);
	}
	return S_OK;
}
bool Player::VAddChild(std::unique_ptr<Actor> kid)
{
	if (kid->VGetTag() == "Weapon")
	{
		Weapon* wp = static_cast<Weapon*>(kid.get());
		int slot = wp->GetWeaponSlot();
		m_WPList[slot] = m_Children.size();
	}

	return Actor::VAddChild(std::move(kid));
}

HRESULT Player::VRenderChildren(Scene * pScene)
{
	if (!Mode) 
		Actor::VRenderChildren(pScene);
	return S_OK;
}
HRESULT Player::VOnUpdate(Scene *pScene, float elapsedMs)
{
	Creature::VOnUpdate(pScene, elapsedMs);

	return E_NOTIMPL;
}
const WeaponResource & Player::GetCurrentWeaponInfo()const 
{
	// TODO: insert return statement here
	assert(m_iCurrentWP >= 0 && m_iCurrentWP <= 5);
	Weapon* wp = static_cast<Weapon*>(m_Children[m_WPList[m_iCurrentWP]].get());
	return wp->GetWeaponInfo();
}
vector<LTBSocket>& Player::GetSockets()
{
	return GetComponent<MeshRenderComponent>(MeshRenderComponent::Name)->GetSockets();
}
void Player::Death()
{
	//GetComponent<AnimationComponent>(AnimationComponent::Name)->PlayAnimation("Stdown-A", 0);
}
void Player::SetPVModel()
{
	if (m_iCurrentWP != -1)
	{
		PlayerView = m_Context->GetSystem<ActorFactory>()->CreateActor("GameAssets\\ACTOR\\PVGroup.xml",nullptr,0);
		GunPlayerView* PVWeapon = static_cast<GunPlayerView*>(PlayerView->VGetChild("PVWeapon"));

		Weapon* wp = static_cast<Weapon*>(m_Children[m_WPList[m_iCurrentWP]].get());
		const string& pvmodel = wp->GetPVFileName();

		ModelCache* pModel = m_Context->GetSystem<Resources>()->GetModel(pvmodel);

		// fix model data
		CharacterResource cr = Game::LoadCharacter(m_Character);
		for (auto& el : pModel->pMeshs)
		{
			if (el->Tex->GetName() == "GameAssets/TEXTURES/Default.png")
			{
				string texPath = cr.PVTexFile[m_Team][el->Name];
				el->Tex = m_Context->GetSystem<Resources>()->GetTexture(texPath);
			}
		}
		// Set component data
		PVWeapon->SetData(pModel);

		PVWeapon->Select();

		VAddChild(std::unique_ptr<Actor>(PlayerView));

		Mode = 1;
	}
}
/*
void Player::EventWeaponData(std::shared_ptr<IEvent> pEvents)
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

void Player::EventCharacterData(std::shared_ptr<IEvent> pEvents)
{
	const EvtData_PlayerCharData *p = static_cast<const EvtData_PlayerCharData*>(pEvents.get());

	const vector<CharacterResource>& wr = p->wr;

	size_t i;
	for (i = 0; i < wr.size(); i++)
		if (wr[i].Name == m_Character) break;

	
	
	if (m_MeshRender == nullptr) return;

	m_RModel= m_Context->GetSystem<Resources>()->GetModel(wr[i].ModelFile[m_Team].c_str());
	if (m_RModel)
	{
		// load texture
		
		vector<std::unique_ptr<SkeMesh>>& ve = m_RModel->pMeshs;
		for (size_t j = 0; j < ve.size(); j++)
		{
			map<string, string> ss = wr[i].TexFile[m_Team];
			const char* pTextureFile = ss[ve[j]->Name].c_str();
			
			ve[j]->Tex = m_Context->GetSystem<Resources>()->GetTexture(pTextureFile);
			m_MeshRender->GetMeshList().push_back(m_RModel->pMeshs[j].get());
		}

		

	}
}
*/