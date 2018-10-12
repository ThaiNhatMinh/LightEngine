#pragma once

#include "..\Interface\ISubSystem.h"
#include "..\Interface\IFactory.h"
#include "..\Interface\IComponent.h"
#include "..\Interface\IActor.h"
#include "..\Interface\IEventManager.h"
#include "..\Graphics3D\Material.h"
#include "..\Interface\IScript.h"
namespace Light
{
	class ActorFactory : public IFactory
	{
		ActorId m_lastActorId;

	protected:
		std::map<std::string, std::function<IComponent*()>> m_ComponentFactoryMap;
		std::map<std::string, std::function<IActor*(int id)>> m_ActorFactoryMap;
		std::map<std::string, std::shared_ptr<render::Material>> m_MaterialMap;
		std::map<std::string, std::function<IScript*(IContext* pContext, IActor* owner)>> m_ScriptMap;

		std::vector<std::unique_ptr<IScene>> m_Scenes;
	public:

		ActorFactory(IContext* c);
		~ActorFactory();
		// Create Actor from	 file
		virtual bool				VRegisterComponentFactory(const std::string& name, std::function<IComponent*()>)override;
		virtual bool				VRegisterActorFactory(const std::string& name, std::function<IActor*(int id)>)override;
		virtual IActor*				VCreateActor(const char* actorResource, bool isCreateChild)override;
		virtual bool				VRegisterMaterial(const std::string& name, std::function<render::Material*()>) override;
		virtual std::shared_ptr<render::Material>	VGetMaterial(const std::string& name) override;
		virtual const char*	VGetName();
		virtual IScene*				VCreateScene(const std::string& name);
		void						PostInit();
	private:
		IComponent * CreateComponent(const tinyxml2::XMLElement* pData);
		ActorId GetNextActorId(void) { ++m_lastActorId; return m_lastActorId; }

	private:
		IEventManager * m_pEventManager;
		IContext* m_pContext;

	};

}