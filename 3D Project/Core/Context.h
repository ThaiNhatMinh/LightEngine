#pragma once
#include <pch.h>

namespace Light
{
	class Context : public IContext
	{
	public:

		Context();
		~Context();

		virtual bool VAddSystem(ISubSystem* system)override;
		virtual void VMakeContext()override;
	private:
		virtual ISubSystem * VGetSystem(const std::type_info& rtti)override;

	private:
		std::vector<ISubSystem*> m_Systems;
	};
}