#include "pch.h"
#include <fmod_errors.h>


SoundEngine::SoundEngine() :m_pListener(nullptr)
{
}

void SoundEngine::Init(Context * c)
{
	FMOD_RESULT result; 

	// Create the Studio System object.
	result = FMOD::System_Create(&m_pSystem);

	if (result!=FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);

	}

	// Initialize FMOD Studio, which will also initialize FMOD Low Level
	result = m_pSystem->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}

	m_pSystem->set3DSettings(1.0f, 1.0f, 1.0f);
	c->m_pSoundEngine = std::unique_ptr<SoundEngine>(this);
	
	/*FMOD::Sound *pSound;
	if (m_pSystem->createSound("GameAssets\\SOUND\\AI_BOOSTINGFULL.WAV", FMOD_DEFAULT, 0, &pSound) != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));

	}

	m_pSystem->playSound(pSound, 0, 0, 0);*/
}

void SoundEngine::ShutDown()
{
	m_pSystem->release();
}

void SoundEngine::SetListener(SoundListener * listener)
{
	m_pListener = listener;
}

void SoundEngine::Update()
{
	assert(m_pListener != nullptr);
	SoundListener::ListenerAttribute attribute = m_pListener->GetListenAttribute();
	m_pSystem->set3DListenerAttributes(attribute.id, &attribute.pos, nullptr, &attribute.forward, &attribute.up);
	m_pSystem->update();
}

FMOD::System * SoundEngine::GetFMODSystem()
{
	return m_pSystem;
}

