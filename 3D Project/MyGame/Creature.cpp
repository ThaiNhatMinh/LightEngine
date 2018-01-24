#include "pch.h"

bool Creature::Init(const tinyxml2::XMLElement * pData)
{
	const tinyxml2::XMLElement* pInfo = pData->FirstChildElement("Creature");
	if (pInfo)
	{
		m_HP = pInfo->Int64Attribute("HP", 0);
		m_AC = pInfo->Int64Attribute("AC", 0);
	}
	else
	{
		E_ERROR("Can't load Creature info");
	}

	return Actor::Init(pData);
}

bool Creature::ChangeHP(int delta)
{
	m_HP += delta;
	return 1;
}

bool Creature::ChangeAC(int newAC)
{
	m_AC += newAC;
	return 1;
}
