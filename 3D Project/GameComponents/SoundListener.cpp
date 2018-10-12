#include <pch.h>
#include "SoundListener.h"
#include "..\interface.h"
namespace Light
{

	FMOD_VECTOR ToFMODVector(vec3 v)
	{
		FMOD_VECTOR fv;
		fv.x = v.x;
		fv.y = v.y;
		fv.z = v.z;
		return fv;
	}

	bool SoundListener::VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData)
	{
		pContext->GetSystem<ISoundEngine>()->VSetListener(this);
		return true;
	}

	tinyxml2::XMLElement * SoundListener::VDeserialize(tinyxml2::XMLDocument * p)
	{
		return nullptr;
	}

	SoundListener::ListenerAttribute SoundListener::GetListenAttribute()
	{
		ListenerAttribute attribute;
		attribute.id = 0; // default 0 for now
		attribute.pos = ToFMODVector(m_pOwner->VGetGlobalTransform()[3]);
		attribute.vel = ToFMODVector(vec3(0, 0, 0));
		attribute.forward = ToFMODVector(m_pOwner->VGetGlobalTransform()[2]);
		attribute.up = ToFMODVector(m_pOwner->VGetGlobalTransform()[1]);
		return attribute;
	}
}