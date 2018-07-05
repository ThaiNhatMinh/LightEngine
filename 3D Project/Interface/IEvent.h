#pragma once

typedef unsigned long EventType;
typedef fastdelegate::FastDelegate1<std::shared_ptr<IEvent>> EventListenerDelegate;



class IEvent
{
public:
	virtual ~IEvent(void) {}
	virtual const EventType& VGetEventType(void) const = 0;
	virtual void VSerialize(std::ostrstream& out) const = 0;
	virtual void VDeserialize(std::istrstream& in) = 0;
	virtual const char* GetName(void) const = 0;

};