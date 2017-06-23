#include "pch.h"
#include "TestScriptEvent.h"

const EventType EvtData_ScriptEventTest_ToLua::sk_EventType(0x5b5b28f3);
void EvtData_ScriptEventTest_ToLua::VBuildEventData(void)
{
	m_eventData.AssignNumber(gLuaState()->GetLuaState(), m_num);
}