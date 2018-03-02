#include "stdafx.h"

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

int Creature::GetHP()
{
	return m_HP;
}

int Creature::GetAC()
{
	return m_AC;
}

void Creature::Death()
{

}

void Creature::TakeDamage(int damage)
{
	m_HP -= damage;

	if (m_HP <= 0) Death();
}
