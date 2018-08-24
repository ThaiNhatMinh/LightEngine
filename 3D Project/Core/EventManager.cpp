#include <pch.h>
#include "EventManager.h"
namespace Light
{


	EventManager::EventManager(IContext* c)
	{
		
		c->VAddSystem(this);
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


	//---------------------------------------------------------------------------------------------------------------------
	// EventManager::VAddListener
	//---------------------------------------------------------------------------------------------------------------------
	bool EventManager::VAddListener(IEventDelegate* eventDelegate, const EventType& type)
	{
		//E_DEBUG(("Events Attempting to add delegate function for event type: " + ToStr(type, 16)));

		EventListenerList& eventListenerList = m_eventListeners[type];  // this will find or create the entry
		for (auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
		{
			if (eventDelegate == (*it).get())
			{
				E_WARNING("Attempting to double-register a delegate");
				return false;
			}
		}

		eventListenerList.push_back(std::unique_ptr<IEventDelegate>(eventDelegate));
		return true;
	}


	//---------------------------------------------------------------------------------------------------------------------
	// EventManager::VRemoveListener
	//---------------------------------------------------------------------------------------------------------------------
	bool EventManager::VRemoveListener(IEventDelegate* eventDelegate, const EventType& type)
	{
		//E_DEBUG("Events Attempting to remove delegate function from event type: " + ToStr(type, 16));
		bool success = false;

		auto findIt = m_eventListeners.find(type);
		if (findIt != m_eventListeners.end())
		{
			EventListenerList& listeners = findIt->second;
			for (auto it = listeners.begin(); it != listeners.end(); ++it)
			{
				if (eventDelegate == (*it).get())
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
				IEventDelegate* listener = (*it).get();
				//E_DEBUG("Events Sending Event " + std::string(pEvent->GetName()) + " to delegate.");
				listener->Invoke(pEvent);  // call the delegate
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
			E_ERROR("Invalid event in VQueueEvent()");

			return false;
		}

		//E_DEBUG("Events Attempting to queue event: " + std::string(pEvent->GetName()));


		auto findIt = m_eventListeners.find(pEvent->VGetEventType());
		if (findIt != m_eventListeners.end())
		{
			m_queues.push_back(pEvent);

			return true;
		}
		else
		{
			E_WARNING("Events Skipping event since there are no delegates registered to receive it: %s",pEvent->GetName());
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
			EventQueue& eventQueue = m_queues;
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

		


		// Process the queue
		while (!m_queues.empty())
		{
			// pop the front of the queue
			std::shared_ptr<IEvent> pEvent = m_queues.front();
			m_queues.pop_front();
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
					IEventDelegate* listener = (*it).get();
					listener->Invoke(pEvent);

				}
			}
			else
			{
				E_ERROR("No listen found.");
			}


			// check to see if time ran out
			currMs = GetTickCount();
			if (maxMillis != IEventManager::kINFINITE && currMs >= maxMs)
			{
				E_DEBUG("EventLoop Aborting event processing; time ran out");
				break;
			}
		}
		return true;
	}

	const char * EventManager::VGetName()
	{
		return typeid(IEventManager).name();
	}

}
