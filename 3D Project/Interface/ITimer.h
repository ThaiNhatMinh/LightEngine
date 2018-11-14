#pragma once


namespace Light
{
	class ITimer : public ISubSystem
	{
	public:
		virtual float	VGetGameTime() const = 0; // in second
		virtual float	VGetDeltaTime() const = 0; // in second
		virtual int		VGetFPS() = 0;
	};
}