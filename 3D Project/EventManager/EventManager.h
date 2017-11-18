#pragma once
#include "pch.h"


//---------------------------------------------------------------------------------------------------------------------
// Macro for event registration
//---------------------------------------------------------------------------------------------------------------------
extern GenericObjectFactory<IEvent, EventType> g_eventFactory;
#define REGISTER_EVENT(eventClass) g_eventFactory.Register<eventClass>(eventClass::sk_EventType)
#define CREATE_EVENT(eventType) g_eventFactory.Create(eventType)

const unsigned int EVENTMANAGER_NUM_QUEUES = 2;

class EventManager : public IEventManager, public Singleton<EventManager>
{
	typedef std::list<EventListenerDelegate> EventListenerList;
	typedef std::map<EventType, EventListenerList> EventListenerMap;
	typedef std::list<const IEvent*> EventQueue;

	EventListenerMap m_eventListeners;
	EventQueue m_queues[EVENTMANAGER_NUM_QUEUES];
	int m_activeQueue;  // index of actively processing queue; events enque to the opposing queue

	//ThreadSafeEventQueue m_realtimeEventQueue;
	

	

public:
	
	EventManager();
	virtual ~EventManager(void);

	virtual void onStartUp();
	virtual void onShutDown();

	virtual bool VAddListener(const EventListenerDelegate& eventDelegate, const EventType& type);
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type);

	virtual bool VTriggerEvent(const IEvent* pEvent) const;
	virtual bool VQueueEvent(const IEvent* pEvent);
	virtual bool VThreadSafeQueueEvent(const IEvent* pEvent);
	virtual bool VAbortEvent(const EventType& type, bool allOfType = false);

	virtual bool VUpdate(unsigned long maxMillis = kINFINITE);
};


IEventManager* gEventManager();