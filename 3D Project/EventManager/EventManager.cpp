#include "pch.h"

GenericObjectFactory<IEvent, EventType> g_eventFactory;


EventManager::EventManager()
{
	m_activeQueue = 0;
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::~EventManager
//---------------------------------------------------------------------------------------------------------------------
EventManager::~EventManager()
{
	// find event not fire and deleteit
	//IEvent* pEvent = nullptr;
	//while (!m_queues[0].empty() && (pEvent = m_queues[0].front()) != nullptr) deletepEvent;
	//while (!m_queues[0].empty() && (pEvent = m_queues[1].front()) != nullptr) deletepEvent;
}

void EventManager::Init(Context* c)
{
	//E_DEBUG("Event Manager Initialize...");
	c->AddSystem(this);
}

void EventManager::ShutDown()
{
	E_DEBUG("Event Manager Shutdown...");
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VAddListener
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::VAddListener(const EventListenerDelegate& eventDelegate, const EventType& type)
{
	//E_DEBUG(("Events Attempting to add delegate function for event type: " + ToStr(type, 16)));

	EventListenerList& eventListenerList = m_eventListeners[type];  // this will find or create the entry
	for (auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
	{
		if (eventDelegate == (*it))
		{
			E_WARNING("Attempting to double-register a delegate");
			return false;
		}
	}

	eventListenerList.push_back(eventDelegate);
	//E_DEBUG("Events Successfully added delegate for event type: " + ToStr(type, 16));

	return true;
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VRemoveListener
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type)
{
	//E_DEBUG("Events Attempting to remove delegate function from event type: " + ToStr(type, 16));
	bool success = false;

	auto findIt = m_eventListeners.find(type);
	if (findIt != m_eventListeners.end())
	{
		EventListenerList& listeners = findIt->second;
		for (auto it = listeners.begin(); it != listeners.end(); ++it)
		{
			if (eventDelegate == (*it))
			{
				listeners.erase(it);
				//E_DEBUG("Events Successfully removed delegate function from event type: " + ToStr(type, 16));
				success = true;
				break;  // we don't need to continue because it should be impossible for the same delegate function to be registered for the same event more than once
			}
		}
	}

	return success;
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VTrigger
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::VTriggerEvent(std::shared_ptr<IEvent> pEvent) const
{
	//E_DEBUG("Events Attempting to trigger event " + std::string(pEvent->GetName()));
	bool processed = false;

	auto findIt = m_eventListeners.find(pEvent->VGetEventType());
	if (findIt != m_eventListeners.end())
	{
		const EventListenerList& eventListenerList = findIt->second;
		for (EventListenerList::const_iterator it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
		{
			EventListenerDelegate listener = (*it);
			//E_DEBUG("Events Sending Event " + std::string(pEvent->GetName()) + " to delegate.");
			listener(pEvent);  // call the delegate
			processed = true;
		}
	}


	return processed;
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VQueueEvent
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::VQueueEvent(std::shared_ptr<IEvent> pEvent)
{
	//GCC_ASSERT(m_activeQueue >= 0);
	//GCC_ASSERT(m_activeQueue < EVENTMANAGER_NUM_QUEUES);

	// make sure the event is valid
	if (!pEvent)
	{
		//E_ERROR("Invalid event in VQueueEvent()");
		
		return false;
	}

	//E_DEBUG("Events Attempting to queue event: " + std::string(pEvent->GetName()));

	auto findIt = m_eventListeners.find(pEvent->VGetEventType());
	if (findIt != m_eventListeners.end())
	{
		m_queues[m_activeQueue].push_back(pEvent);
		//E_DEBUG("Events Successfully queued event: " + std::string(pEvent->GetName()));
		return true;
	}
	else
	{
		//E_DEBUG("Events Skipping event since there are no delegates registered to receive it: " + std::string(pEvent->GetName()));
		
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VThreadSafeQueueEvent
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::VThreadSafeQueueEvent(std::shared_ptr<IEvent>)
{
	//m_realtimeEventQueue.push(pEvent);
	return true;
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VAbortEvent
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::VAbortEvent(const EventType& inType, bool allOfType)
{
	//GCC_ASSERT(m_activeQueue >= 0);
	//GCC_ASSERT(m_activeQueue < EVENTMANAGER_NUM_QUEUES);

	bool success = false;
	EventListenerMap::iterator findIt = m_eventListeners.find(inType);

	if (findIt != m_eventListeners.end())
	{
		EventQueue& eventQueue = m_queues[m_activeQueue];
		auto it = eventQueue.begin();
		while (it != eventQueue.end())
		{
			// Removing an item from the queue will invalidate the iterator, so have it point to the next member.  All
			// work inside this loop will be done using thisIt.
			auto thisIt = it;
			++it;

			if ((*thisIt)->VGetEventType() == inType)
			{
				eventQueue.erase(thisIt);
				success = true;
				if (!allOfType)
					break;
			}
		}
	}

	return success;
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VTick
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::VUpdate(unsigned long maxMillis)
{
	unsigned long currMs = GetTickCount();
	unsigned long maxMs = ((maxMillis == IEventManager::kINFINITE) ? (IEventManager::kINFINITE) : (currMs + maxMillis));

	// This section added to handle events from other threads.  Check out Chapter 20.
	/*IEventDataPtr pRealtimeEvent;
	while (m_realtimeEventQueue.try_pop(pRealtimeEvent))
	{
		VQueueEvent(pRealtimeEvent);

		currMs = GetTickCount();
		if (maxMillis != IEventManager::kINFINITE)
		{
			if (currMs >= maxMs)
			{
				GCC_ERROR("A realtime process is spamming the event manager!");
			}
		}
	}*/

	// swap active queues and clear the new queue after the swap
	int queueToProcess = m_activeQueue;
	m_activeQueue = (m_activeQueue + 1) % EVENTMANAGER_NUM_QUEUES;
	m_queues[m_activeQueue].clear();


	// Process the queue
	while (!m_queues[queueToProcess].empty())
	{
		// pop the front of the queue
		std::shared_ptr<IEvent> pEvent = m_queues[queueToProcess].front();
		m_queues[queueToProcess].pop_front();
		//E_DEBUG("EventLoop \t\tProcessing Event " + std::string(pEvent->GetName()));

		const EventType& eventType = pEvent->VGetEventType();

		// find all the delegate functions registered for this event
		auto findIt = m_eventListeners.find(eventType);
		if (findIt != m_eventListeners.end())
		{
			const EventListenerList& eventListeners = findIt->second;
			//E_DEBUG("EventLoop \t\tFound " + ToStr((unsigned long)eventListeners.size()) + " delegates");

			// call each listener
			for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it)
			{
				EventListenerDelegate listener = (*it);
				//E_DEBUG("EventLoop \t\tSending event " + std::string(pEvent->GetName()) + " to delegate");
				listener(pEvent);
			}
		}

		
		// check to see if time ran out
		currMs = GetTickCount();
		if (maxMillis != IEventManager::kINFINITE && currMs >= maxMs)
		{
			E_DEBUG("EventLoop Aborting event processing; time ran out");
			break;
		}
	}

	// If we couldn't process all of the events, push the remaining events to the new active queue.
	// Note: To preserve sequencing, go back-to-front, inserting them at the head of the active queue
	bool queueFlushed = (m_queues[queueToProcess].empty());
	if (!queueFlushed)
	{
		while (!m_queues[queueToProcess].empty())
		{
			std::shared_ptr<IEvent> pEvent = m_queues[queueToProcess].back();
			m_queues[queueToProcess].pop_back();
			m_queues[m_activeQueue].push_front(pEvent);
		}
	}

	return queueFlushed;
}

