#pragma once
#include <string>
#include "..\Graphics3D\ICamera.h"
#include "IActor.h"

namespace Light
{
	class IScene
	{
	public:
		~IScene() = default;
		virtual bool				VLoadScene(const std::string& filename) = 0;
		virtual bool				VOnRender() = 0;

		virtual bool				VOnUpdate(float dt) = 0;
		virtual bool				VPostUpdate() = 0;

		virtual render::ICamera*	VGetCurrentCamera() = 0;
		virtual void				VSetCurrentCamera(render::ICamera * cam) = 0;
		virtual IActor*				VGetRoot() = 0;
	};
}