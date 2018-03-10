#pragma once

#include <fmod.hpp>

FMOD_VECTOR ToFMODVector(vec3 v);
class SoundListener;

class SoundEngine :public ISubSystem
{
public:
	
	SoundEngine(Context* c);
	~SoundEngine();
	
	void SetListener(SoundListener* listener);
	void Update();
	FMOD::System* GetFMODSystem();
private:
	FMOD::System* m_pSystem;
	SoundListener* m_pListener;
};

