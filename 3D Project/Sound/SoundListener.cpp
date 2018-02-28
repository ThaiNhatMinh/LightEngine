#include "pch.h"

const char* SoundListener::Name = "SoundListener";

FMOD_VECTOR ToFMODVector(vec3 v)
{
	FMOD_VECTOR fv;
	fv.x = v.x;
	fv.y = v.y;
	fv.z = v.z;
	return fv;
}
bool SoundListener::VInit(const tinyxml2::XMLElement * pData)
{
	m_Context->GetSystem<SoundEngine>()->SetListener(this);
	return true;
}
void SoundListener::VUpdate(float dt)
{
	
}


const char * SoundListener::VGetName() const
{
	return Name;
}

SoundListener::ListenerAttribute SoundListener::GetListenAttribute()
{
	ListenerAttribute attribute;
	attribute.id = 0; // default 0 for now
	attribute.pos = ToFMODVector(m_pOwner->VGetTransform()[3]);
	attribute.vel = ToFMODVector(vec3(0, 0, 0));
	attribute.forward = ToFMODVector(m_pOwner->VGetTransform()[2]);
	attribute.up = ToFMODVector(m_pOwner->VGetTransform()[1]);
	return attribute;
}
