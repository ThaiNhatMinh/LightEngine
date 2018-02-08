#include "pch.h"

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

EvtRequestCreateSprite::EvtRequestCreateSprite(const string & file, const vec3 & pos) :Parent(nullptr),File(file),Pos(pos),Loop(0) {}

const vec3 & EvtRequestCreateSprite::GetPos()
{
	// TODO: insert return statement here
	return Pos;
}

Actor * EvtRequestCreateSprite::GetParent()
{
	return Parent;
}

void EvtRequestCreateSprite::SetParent(Actor * p)
{
	Parent = p;
}

void EvtRequestCreateSprite::SetLoop(bool loop)
{
	Loop = loop;
}

bool EvtRequestCreateSprite::isLoop()
{
	return Loop;
}

const string & EvtRequestCreateSprite::GetFile()
{
	// TODO: insert return statement here
	return File;
}
