#pragma once

namespace Light
{
	class ISubSystem
	{
		friend class ActorFactory;
		friend class Application;
	public:

		virtual ~ISubSystem() {};
		virtual const char* VGetName() = 0;
	protected:
		ISubSystem() {};
		ISubSystem& operator=(const ISubSystem& other) { return *this; }
		ISubSystem(const ISubSystem& other) {}
	};
}