#pragma once

#include "..\Interface\ISubSystem.h"
#include "..\Interface\IComponent.h"

#include <functional>
namespace Light
{
	class IScriptManager : public ISubSystem
	{
	public:
		
		virtual void BindComponent(IScriptComponent*) = 0;
		virtual void Start() = 0;
		virtual void Update(float dt) = 0;
	};
}