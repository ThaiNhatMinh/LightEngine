#pragma once

#include "..\Interface\IEventManager.h"

//---------------------------------------------------------------------------------------------------------------------
// Macro for event registration
//---------------------------------------------------------------------------------------------------------------------
namespace Light
{
	class EventManager : public IEventManager
	{
		typedef std::list<std::unique_ptr<IEventDelegate>> EventListenerList;
		typedef std::map<EventType, EventListenerList> EventListenerMap;
		typedef std::list<std::shared_ptr<IEvent>> EventQueue;

		EventListenerMap m_eventListeners;
		EventQueue m_queues;
		public:

		EventManager(IContext* c);
		virtual ~EventManager(void);


		virtual bool VAddListener(IEventDelegate* eventDelegate, const EventType& type);
		virtual bool VRemoveListener(IEventDelegate* eventDelegate, const EventType& type);

		virtual bool VTriggerEvent(std::shared_ptr<IEvent> pEvent) const;
		virtual bool VQueueEvent(std::shared_ptr<IEvent> pEvent);
		virtual bool VThreadSafeQueueEvent(std::shared_ptr<IEvent> pEvent);
		virtual bool VAbortEvent(const EventType& type, bool allOfType = false);

		virtual bool VUpdate(unsigned long maxMillis = kINFINITE);

		virtual const char* VGetName();
	};

}