#pragma once


namespace Light
{
	class ITimer : public ISubSystem
	{
	public:
		virtual void	VReset() = 0;	// call before message loop
		virtual void	VStart() = 0;	// call when unpause
		virtual void	VStop() = 0;	// call when pause
		virtual void	VTick() = 0;	// call every frame
		virtual float	VGetGameTime() const = 0; // in second
		virtual float	VGetDeltaTime() const = 0; // in second
		virtual int		VGetFPS() = 0;
	};
}