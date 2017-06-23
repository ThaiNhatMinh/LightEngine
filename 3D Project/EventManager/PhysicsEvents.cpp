#include "pch.h"
#include "PhysicsEvents.h"

// {C74B2E59-77A3-4EFA-8210-4A94C7FA9E49}
// {C879C281-9E31-4C52-A363-8229D3B4B23E}
// {DEC84258-0653-4F48-9996-0E59DAAE996F}
// {FA8D43FB-6DC7-4B0B-8533-A77BF867DD14}
const EventType EvtData_PhysTrigger_Enter::sk_EventType(0xc74b2e59);
const EventType EvtData_PhysTrigger_Leave::sk_EventType(0xC879C281);
const EventType EvtData_PhysCollision::sk_EventType(0xDEC84258);
const EventType EvtData_PhysSeparation::sk_EventType(0xFA8D43FB);

void EvtData_PhysCollision::VBuildEventData(void)
{
	m_eventData.AssignNewTable(gLuaState()->GetLuaState());
	m_eventData.SetInteger("actorA", m_ActorA);
	m_eventData.SetInteger("actorB", m_ActorB);
}
