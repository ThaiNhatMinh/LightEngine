#pragma once

#include <fmod.hpp>
#include "..\interface.h"
#include "..\GameComponents\SoundListener.h"
//FMOD_VECTOR ToFMODVector(vec3 v);

namespace Light
{

	class SoundEngine :public ISoundEngine
	{
	public:

		SoundEngine(IContext* c);
		~SoundEngine();

		void VSetListener(ISoundListener* listener);
		void VUpdate();
		const char* VGetName();
		FMOD::System* GetFMODSystem();
	private:
		FMOD::System* m_pSystem;
		SoundListener::ListenerAttribute m_Attribute;
		ISoundListener* m_pListener;
	};
}
