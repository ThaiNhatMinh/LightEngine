
#pragma once
#include "..\Interface\ITimer.h"
namespace Light
{
	class GameTimer : public ITimer
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
		GameTimer(IContext* c);
		~GameTimer();

		virtual float VGetGameTime() const; // in second
		virtual float VGetDeltaTime() const; // in second
		virtual int VGetFPS();

		void VReset();	// call before message loop
		void VStart();	// call when unpause
		void VStop();	// call when pause
		void VTick();	// call every frame
		virtual const char* VGetName();

	};
}