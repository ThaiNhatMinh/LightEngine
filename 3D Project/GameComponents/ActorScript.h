#pragma once
#include <pch.h>

// C++ class using for Lua Script
class ActorSctipt
{
public:
	// Call before game loop begin
	void	Start();
	void	Finish();
	ActorId GetActorID();
};