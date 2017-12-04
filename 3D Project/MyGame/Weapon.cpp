#include "pch.h"

Weapon::Weapon(ActorId id):Actor(id)
{
}

bool Weapon::Init(const tinyxml2::XMLElement * pData)
{
	return Actor::Init(pData);
}

void Weapon::PostInit(void)
{
	Actor::PostInit();
}

HRESULT Weapon::VRender(Scene * pScene)
{
	return E_NOTIMPL;
}

int Weapon::GetWeaponIndex()
{
	return m_Index;
}
