tf = GetComponent("Transform")


pos = tf:GetPos()
print(pos.x, pos.y, pos.z)
print(Event.EventType.EvtKeyEvent)
print(Input.KeyAction.Release)


function KeyPress(evt)
	print(evt.key, evt.action)
end

RegisterEvent(Event.EventType.EvtKeyEvent, KeyPress)
