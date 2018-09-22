#pragma once

#include <sol.hpp>
#include "..\Interface\IEvent.h"
namespace Light
{
	class EventListener
	{
	public:
		sol::function callback;
		EventType type;

		virtual ~EventListener() = default;
	};
	template<class T>
	class ScriptEventListener: public EventListener
	{
	
	public:
		void OnEvent(std::shared_ptr<IEvent> event);
	};
	template<class T>
	inline void ScriptEventListener<T>::OnEvent(std::shared_ptr<IEvent> event)
	{
		T* pEvent = static_cast<T*>(event.get());

		callback(pEvent);
	}
}