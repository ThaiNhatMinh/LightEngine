#pragma once
#include <memory>
#include <iostream>
#include <strstream>
#include "..\typedef.h"

namespace Light
{
	class IEvent
	{
	public:
		virtual ~IEvent(void) {}
		virtual const EventType& VGetEventType(void) const = 0;
		virtual void VSerialize(std::ostrstream& out) const {};
		virtual void VDeserialize(std::istrstream& in) {};
		virtual const char* GetName(void) const = 0;

	};
	
	template<class T>
	class Event: public IEvent
	{
	public:
		static const EventType Type;
		virtual const EventType& VGetEventType(void) const final
		{
			return typeid(T).hash_code();
		}
		virtual const char* GetName(void) const final
		{
			return typeid(T).name();
		}
	};

	template<class T> EventType Event<T>::Type = typeid(T).hash_code();

	class IEventDelegate
	{
	public:
		virtual ~IEventDelegate() = default;
		virtual inline void Invoke(std::shared_ptr<IEvent> eventData) = 0;
		virtual inline EventDelegateType GetType() = 0;
		virtual bool operator==(const IEventDelegate* other) const = 0;
	};


	template<class T>
	class EventDelegate : public IEventDelegate
	{
	private:
		typedef void(T::*Callback)(std::shared_ptr<IEvent>);

		T* m_Receiver;
		Callback m_Callback;
	public:
		EventDelegate(T* receiver, Callback& callback)
		{
			m_Receiver = receiver;
			m_Callback = callback;
		}

		virtual inline void Invoke(std::shared_ptr<IEvent> eventData)
		{
			(m_Receiver->*m_Callback)(eventData);
		}
		virtual inline EventDelegateType GetType()
		{
			static const EventDelegateType DELEGATE_TYPE{ typeid(T).hash_code() ^ typeid(Callback).hash_code() };
			return DELEGATE_TYPE;
		}

		virtual bool operator==(const IEventDelegate* other) const
		{
			if (this->GetType() != other->GetType()) return false;
			EventDelegate* delegate = (EventDelegate*)other;
			if (other == nullptr) return false;

			return (m_Callback == delegate->m_Callback && m_Receiver == delegate->m_Receiver);
		}
	};

	
}

