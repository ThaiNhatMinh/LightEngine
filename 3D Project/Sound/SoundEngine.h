#pragma once

#include <fmod.hpp>

class SoundEngine :public ISubSystem
{
public:
	SoundEngine() = default;
	~SoundEngine() = default;
	virtual void Init(Context* c);
	virtual void ShutDown();

	void Update();

private:
	FMOD::System* m_pSystem;
};

