#include "pch.h"
#include "Player.h"

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
	const tinyxml2::XMLElement * pWeapon = pData->FirstChildElement("Weapon");

	int i = 1;
	while (1)
	{
		string name = "Name" + ('0'+i);
		const char* Name = pWeapon->Attribute(name.c_str());
		if (!Name) break;
		std::shared_ptr<const IEvent> pEvent(new EvtData_RequestCreateWeapon(this, Name));
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
}

HRESULT Player::VRender(Scene * pScene)
{
	MeshRenderComponent* mrc = GetComponent<MeshRenderComponent>("MeshRenderComponent");
	if (mrc) mrc->Render(pScene);
	return S_OK;
}

void Player::EventWeaponData(std::shared_ptr<const IEvent> pEvents)
{
	const EvtData_PlayerWpData *p = static_cast<const EvtData_PlayerWpData*>(pEvents.get());
	const vector<WeaponResource>& wr = p->wr;
	for (int i = 0; i < m_Children.size(); i++)
	{
		if (m_Children[i]->VGetTag() == "Weapon")
		{
			Weapon* pWp = static_cast<Weapon*>(m_Children[i].get());
			int id = pWp->GetWeaponIndex();
			pWp->LoadData(wr[id]);
			
		}
	}
	
}

void Player::EventCharacterData(std::shared_ptr<const IEvent> pEvents)
{
}
