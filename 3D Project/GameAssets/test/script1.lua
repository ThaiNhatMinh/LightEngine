tf = GetComponent("Transform")


pos = tf:GetPos()
pos = pos + glm.vec3.new(100,0,0)
--tf:SetPos(pos)

print(pos.x, pos.y, pos.z)
print(Event.EventType.EvtKeyEvent)
print(Input.KeyAction.Release)


function KeyPress(evt)
	print(evt.key, evt.action)
end

function MouseMove(evt)
	print(evt.dx, evt.dy)
end


--RegisterEvent(Event.EventType.EvtKeyEvent, KeyPress)
--RegisterEvent(Event.EventType.EvtMouseMove, MouseMove)
