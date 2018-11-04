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
		virtual void VMakeContext()override;
		virtual void VReleaseContext()override;
	private:
		virtual ISubSystem * VGetSystem(const std::type_info& rtti)override;

	private:
		std::mutex m_Lock;
		void* pthread;
		std::vector<ISubSystem*> m_Systems;
	};
}