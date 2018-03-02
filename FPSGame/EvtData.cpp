#include "stdafx.h"

const EventType EvtTakeDamage::sk_EventType(0x24f8d4b3);

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

