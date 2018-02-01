#pragma once
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

class SoundEngine :public ISubSystem
{
public:
	SoundEngine() = default;
	~SoundEngine() = default;
	virtual void Init(Context* c);
	virtual void ShutDown();

private:
	ALCdevice* m_pSoundDevice;
	ALCcontext* m_pSoundContext;
};