#pragma once
#include <pch.h>
#include <mutex>

namespace Light
{
	class Context : public IContext
	{
	public:

		Context();
		~Context();

		virtual bool VAddSystem(ISubSystem* system)override;
		virtual void VExit()override;

		bool Exiting();
	private:
		virtual ISubSystem * VGetSystem(const std::type_info& rtti)override;

	private:
		std::vector<ISubSystem*> m_Systems;
		bool m_bExit;
	};
}