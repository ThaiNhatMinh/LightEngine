#pragma once

#include "..\Interface\ISubSystem.h"
#include "..\Interface\IFactory.h"
#include "..\Interface\IComponent.h"
#include "..\Interface\IActor.h"
#include "..\Interface\IEventManager.h"

namespace Light
{
	class ActorFactory : public IFactory
	{
		ActorId m_lastActorId;

	protected:
		std::map<std::string, std::function<IComponent*()>> m_ComponentFactoryMap;
		std::map<std::string, std::function<IActor*(int id)>> m_ActorFactoryMap;
		
	public:

		ActorFactory(IContext* c);
		~ActorFactory();
		// Create Actor from file
		virtual bool		VRegisterComponentFactory(string name, std::function<IComponent*()>)override;
		virtual bool		VRegisterActorFactory(const string& name, std::function<IActor*(int id)>)override;
		virtual IActor*		VCreateActor(const char* actorResource, bool isCreateChild)override;


	private:
		IComponent * CreateComponent(const tinyxml2::XMLElement* pData);
		ActorId GetNextActorId(void) { ++m_lastActorId; return m_lastActorId; }

	private:
		IEventManager * m_pEventManager;
		IContext* m_pContext;
	};

}