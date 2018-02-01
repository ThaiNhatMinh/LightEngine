#include "pch.h"

void SoundEngine::Init(Context * c)
{
	m_pSoundDevice = alcOpenDevice(nullptr);

	if (m_pSoundDevice)
	{
		m_pSoundContext = alcCreateContext(m_pSoundDevice, NULL);
		alcMakeContextCurrent(m_pSoundContext);
	}

	

}

void SoundEngine::ShutDown()
{
}
