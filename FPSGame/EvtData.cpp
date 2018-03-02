#include "stdafx.h"

const EventType EvtTakeDamage::sk_EventType(0x24f8d4b3);
const EventType EvtExplosion::sk_EventType(0x380fb3d6);

EvtTakeDamage::EvtTakeDamage(Creature * attacker, Creature * victim, int damage):Attacker(attacker),Victim(victim),Damage(damage)
{
}

Creature * EvtTakeDamage::GetAttacker()
{
	return Attacker;
}

Creature * EvtTakeDamage::GetVictim()
{
	return Victim;
}

int EvtTakeDamage::GetDamage()
{
	return Damage;
}

EvtExplosion::EvtExplosion(const vec3 & pos, int damage, float range1, float range2):Pos(pos),Damage(damage),Range1(range1),Range2(range2)
{
}

int EvtExplosion::GetDamage()
{
	return Damage;
}

float EvtExplosion::GetRange1()
{
	return Range1;
}

float EvtExplosion::GetRange2()
{
	return Range2;
}

const vec3 & EvtExplosion::GetPos()
{
	return Pos;
}
