#pragma once

#include <fmod.hpp>

FMOD_VECTOR ToFMODVector(vec3 v);
class SoundListener;

class SoundEngine :public ISubSystem
{
public:
	
	SoundEngine();
	~SoundEngine() = default;
	virtual void Init(Context* c);
	virtual void ShutDown();

	void SetListener(SoundListener* listener);
	void Update();
	FMOD::System* GetFMODSystem();
private:
	FMOD::System* m_pSystem;
	SoundListener* m_pListener;
};

