#pragma once
#include <pch.h>

//---------------------------------------------------------------------------------------------------------------------
// class EvtData_ScriptEventTest				             - Chapter 10, 279
//---------------------------------------------------------------------------------------------------------------------
class EvtData_ScriptEventTest_ToLua : public ScriptEvent
{
	int m_num;

public:
	static const EventType sk_EventType;

	EvtData_ScriptEventTest_ToLua(void) { m_num = 0; }
	EvtData_ScriptEventTest_ToLua(int num) { m_num = num; }

	virtual const EventType& VGetEventType(void) const { return sk_EventType; }

	virtual IEvent* VCopy(void) const
	{
		EvtData_ScriptEventTest_ToLua* pCopy(new EvtData_ScriptEventTest_ToLua(m_num));
		pCopy->m_eventData = m_eventData;
		return pCopy;
	}

	virtual const char* GetName(void) const { return "EvtData_ScriptEventTest_ToLua"; }

protected:
	virtual void VBuildEventData(void);
	//virtual bool VBuildEventFromScript(void);

	EXPORT_FOR_SCRIPT_EVENT(EvtData_ScriptEventTest_ToLua);
};