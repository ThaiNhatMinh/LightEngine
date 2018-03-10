
#pragma once
#include "pch.h"

class GameTimer : public ISubSystem
{
private:

	__int64 m_StartTime;
	__int64 m_PauseTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrentTime;
	bool m_bStoped;
	double m_SecondPerCount;
	double m_DeltaTime;
	int m_FPS;
public:
	GameTimer(Context* c);
	~GameTimer();

	float GetGameTime() const; // in second
	float GetDeltaTime() const; // in second

	void Reset();	// call before message loop
	void Start();	// call when unpause
	void Stop();	// call when pause
	void Tick();	// call every frame
	int GetFPS();

};
