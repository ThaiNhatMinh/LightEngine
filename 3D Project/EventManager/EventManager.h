#pragma once
#include <pch.h>


//---------------------------------------------------------------------------------------------------------------------
// Macro for event registration
//---------------------------------------------------------------------------------------------------------------------

const unsigned int EVENTMANAGER_NUM_QUEUES = 2;

class EventManager : public IEventManager, public ISubSystem
{
	typedef std::list<EventListenerDelegate> EventListenerList;
	typedef std::map<EventType, EventListenerList> EventListenerMap;
	typedef std::list<std::shared_ptr<IEvent>> EventQueue;

	EventListenerMap m_eventListeners;
	EventQueue m_queues[EVENTMANAGER_NUM_QUEUES];
	int m_activeQueue;  // index of actively processing queue; events enque to the opposing queue

	//ThreadSafeEventQueue m_realtimeEventQueue;
	

	

public:
	
	EventManager(Context* c);
	virtual ~EventManager(void);


	virtual bool VAddListener(const EventListenerDelegate& eventDelegate, const EventType& type);
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type);

	virtual bool VTriggerEvent(std::shared_ptr<IEvent> pEvent) const;
	virtual bool VQueueEvent(std::shared_ptr<IEvent> pEvent);
	virtual bool VThreadSafeQueueEvent(std::shared_ptr<IEvent> pEvent);
	virtual bool VAbortEvent(const EventType& type, bool allOfType = false);

	virtual bool VUpdate(unsigned long maxMillis = kINFINITE);
};

