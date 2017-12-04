
BoxScript = {}

function BoxScript:Start()
	Log("BoxScript:Start() with id " .. BoxScript:GetActorId());
	Log("Time Start: " .. Timer:GetCurrentTime());
end

function BoxScript:Finish()
	Log("BoxScript:Finish() with id" .. BoxScript:GetActorId());
	Log("Time Finish: " .. Timer:GetCurrentTime());
end


function BoxScript:Update(dt)
	--Log("BoxScript:Update() with dt= " .. dt);
end


Log("BoxScript with id"..BoxScript:GetActorId());