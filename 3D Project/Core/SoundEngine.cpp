#include <pch.h>
#include <fmod.h>
#include <fmod_errors.h>
#include "SoundEngine.h"
#include "..\GameComponents\SoundListener.h"
namespace Light
{
	SoundEngine::SoundEngine(IContext * c) :m_pListener(nullptr)
	{
		FMOD_RESULT result;

		// Create the Studio System object.
		result = FMOD::System_Create(&m_pSystem);

		if (result != FMOD_OK)
		{
			E_ERROR("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			return;
		}

		// Initialize FMOD Studio, which will also initialize FMOD Low Level
		result = m_pSystem->init(512, FMOD_INIT_NORMAL, 0);
		if (result != FMOD_OK)
		{
			E_ERROR("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		}

		m_pSystem->set3DSettings(1.0f, 1.0f, 1.0f);
		c->VAddSystem(this);

		/*FMOD::Sound *pSound;
		if (m_pSystem->createSound("GameAssets\\SOUND\\AI_BOOSTINGFULL.WAV", FMOD_DEFAULT, 0, &pSound) != FMOD_OK)
		{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));

		}

		m_pSystem->playSound(pSound, 0, 0, 0);*/
	}




	SoundEngine::~SoundEngine()
	{
		m_pSystem->release();
	}

	void SoundEngine::VSetListener(ISoundListener * listener)
	{
		m_pListener = listener;
		m_Attribute = static_cast<SoundListener*>(listener)->GetListenAttribute();

	}

	void SoundEngine::VUpdate()
	{
		if (m_pListener == nullptr) return;
		m_Attribute = static_cast<SoundListener*>(m_pListener)->GetListenAttribute();
		m_pSystem->set3DListenerAttributes(m_Attribute.id, &m_Attribute.pos, nullptr, &m_Attribute.forward, &m_Attribute.up);
		m_pSystem->update();
	}

	const char * SoundEngine::VGetName()
	{
		static const char * t = typeid(ISoundEngine).name();
		return t;
	}

	FMOD::System * SoundEngine::GetFMODSystem()
	{
		return m_pSystem;
	}

}