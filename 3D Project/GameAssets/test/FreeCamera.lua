MouseSensitivity = 0.25
Speed = 200
Pitch = 0
Yaw = -90

tf = GetComponent("Transform")


pos = tf:GetPos()






function KeyPress(evt)
	front = tf:GetFront()
	right = tf:GetRight()

	if(evt.key==Input.Key.W)then
		pos = pos + front*Speed*evt.dt
	elseif (evt.key==Input.Key.S) then
		pos = pos - front*Speed*evt.dt
	elseif (evt.key==Input.Key.A) then
		pos = pos + right*Speed*evt.dt
	elseif (evt.key==Input.Key.D) then
		pos = pos - right*Speed*evt.dt
	end

	tf:SetPos(pos)
	--print(pos.x,pos.y,pos.z)
end

function MouseMove(evt)
	print(evt.dx, evt.dy)
end


RegisterEvent(Event.EventType.EvtKeyEvent, KeyPress)
--RegisterEvent(Event.EventType.EvtMouseMove, MouseMove)
