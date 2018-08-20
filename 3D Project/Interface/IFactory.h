#pragma once
#include <string>
#include <functional>
#include "IComponent.h"
#include "IActor.h"
#include "..\Graphics3D\Material.h"
namespace Light
{
	class IFactory: public ISubSystem
	{
	public:
		virtual ~IFactory() = default;

		virtual bool				VRegisterComponentFactory(const std::string& name, std::function<IComponent*()>) = 0;
		virtual bool				VRegisterActorFactory(const std::string& name, std::function<IActor*(int id)>) = 0;
		virtual IActor*				VCreateActor(const char* filePath, bool isCreateChild = false) = 0;
		virtual bool				VRegisterMaterial(const std::string& name, std::function<render::Material*()>) = 0;
		virtual std::shared_ptr<render::Material>	VGetMaterial(const std::string& name) = 0;
		//virtual IShader*		VCreateShader(const char* type, const char* vs, const char* fs) = 0;

	};
}