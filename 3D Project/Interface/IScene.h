#pragma once
#include <string>
#include "ICamera.h"
#include "IActor.h"
#include "..\Utilities\Utility.h"
#include "..\Graphics3D\Light.h"

namespace Light
{
	class IScene
	{
	public:
		virtual ~IScene() = default;
		virtual bool				VOnRender() = 0;
		virtual bool				VOnUpdate(float dt) = 0;
		virtual bool				VPostUpdate() = 0;
		virtual IActor*				VGetRoot() = 0;
		virtual std::string			VGetSceneName() = 0;
	};
}