#pragma once


namespace Light
{
	class ISoundListener;
	class ISoundEngine : public ISubSystem
	{
	public:
		virtual void VSetListener(ISoundListener* listener)=0;
		virtual void VUpdate() = 0;
	};
}