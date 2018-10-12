#include <pch.h>

EvtRequestCreateSprite::EvtRequestCreateSprite(const string & file, const vec3 & pos) :Parent(nullptr), File(file), Pos(pos), Loop(0) {}
const EventType EvtRequestCreateSprite::sk_EventType(0x53442705);
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
