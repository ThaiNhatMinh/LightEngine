#pragma once

namespace Light 
{
	class IEventManager
	{
	public:

		enum eConstants { kINFINITE = 0xffffffff };

		// Registers a delegate function that will get called when the event type is triggered.  Returns true if 
		// successful, false if not.
		virtual bool VAddListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;

		// Removes a delegate / event type pairing from the internal tables.  Returns false if the pairing was not found.
		virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;

		// Fire off event NOW.  This bypasses the queue entirely and immediately calls all delegate functions registered 
		// for the event.
		virtual bool VTriggerEvent(std::shared_ptr<IEvent> pEvent) const = 0;

		// Fire off event.  This uses the queue and will call the delegate function on the next call to VTick(), assuming
		// there's enough time.
		virtual bool VQueueEvent(std::shared_ptr<IEvent> pEvent) = 0;
		virtual bool VThreadSafeQueueEvent(std::shared_ptr<IEvent> pEvent) = 0;

		// Find the next-available instance of the named event type and remove it from the processing queue.  This 
		// may be done up to the point that it is actively being processed ...  e.g.: is safe to happen during event
		// processing itself.
		//
		// if allOfType is true, then all events of that type are cleared from the input queue.
		//
		// returns true if the event was found and removed, false otherwise
		virtual bool VAbortEvent(const EventType& type, bool allOfType = false) = 0;

		// Allow for processing of any queued messages, optionally specify a processing time limit so that the event 
		// processing does not take too long. Note the danger of using this artificial limiter is that all messages 
		// may not in fact get processed.
		//
		// returns true if all messages ready for processing were completed, false otherwise (e.g. timeout )
		virtual bool VUpdate(unsigned long maxMillis = kINFINITE) = 0;

		virtual ~IEventManager(void) {};

	};
}