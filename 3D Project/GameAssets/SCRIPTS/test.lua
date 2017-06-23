

function TestEventHandler(eventData)
	Log("Event Received in Lua: " .. eventData);
	Log("Time Received: " .. Timer:GetCurrentTime());
	--eventData = eventData + 1;
	--QueueEvent(EventType.EvtData_ScriptEventTest_FromLua, eventData);
end


function RegisterListener()
	RegisterEventListener(EventType.EvtData_ScriptEventTest_ToLua, TestEventHandler);
	Log("Script test loaded");

end


RegisterListener();

