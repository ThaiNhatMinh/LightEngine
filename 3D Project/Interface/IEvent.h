#pragma once
#include <memory>

typedef unsigned long EventType;

namespace Light
{
	class IEvent
	{
	public:
		virtual ~IEvent(void) {}
		virtual const EventType& VGetEventType(void) const = 0;
		virtual void VSerialize(std::ostrstream& out) const = 0;
		virtual void VDeserialize(std::istrstream& in) = 0;
		virtual const char* GetName(void) const = 0;

	};
	class EventListenerDelegate
	{
	private:

		typedef void(*ObjectFunc)(void* objPtr,std::shared_ptr<IEvent>);

		void* thisPointer;
		ObjectFunc thisFunc;
	public:

		EventListenerDelegate(void* p, ObjectFunc func) :thisPointer(p), thisFunc(func) {};

		void operator()(std::shared_ptr<IEvent> pEvent)
		{
			thisFunc(thisPointer,pEvent);
		}
	};

}

