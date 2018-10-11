#pragma once
#include "ISubSystem.h"

namespace Light
{
	class IEffectSystem : public ISubSystem
	{
	public:
		virtual void VUpdate(float dt) = 0;
		virtual void			VRender()=0;
	};

}