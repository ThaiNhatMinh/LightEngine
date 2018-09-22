#pragma once
#include "..\Interface\IComponent.h"
#include <fmod.h>

namespace Light
{
	class SoundListener : public ISoundListener
	{
	public:
		struct ListenerAttribute
		{
			int id;
			FMOD_VECTOR pos;
			FMOD_VECTOR vel;
			FMOD_VECTOR forward;
			FMOD_VECTOR up;
		};

		SoundListener() = default;
		~SoundListener() = default;

		virtual bool VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
		virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);
		
		
		ListenerAttribute GetListenAttribute();

	};
}