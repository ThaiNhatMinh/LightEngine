#pragma once
#include <pch.h>


class EvtData_PhysTrigger_Enter : public BaseEventData
{
	int m_triggerID;
	ActorId m_other;

public:
	static const EventType sk_EventType;

	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_PhysTrigger_Enter()
	{
		m_triggerID = -1;
		m_other = 0;
	}

	explicit EvtData_PhysTrigger_Enter(int triggerID, ActorId other)
		: m_triggerID(triggerID),
		m_other(other)
	{}

	IEvent* VCopy() const
	{
		return new EvtData_PhysTrigger_Enter(m_triggerID, m_other);
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_PhysTrigger_Enter";
	}

	int GetTriggerId(void) const
	{
		return m_triggerID;
	}

	ActorId GetOtherActor(void) const
	{
		return m_other;
	}
};

class EvtData_PhysTrigger_Leave : public BaseEventData
{
	int m_triggerID;
	ActorId m_other;

public:
	static const EventType sk_EventType;

	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_PhysTrigger_Leave()
	{
		m_triggerID = -1;
		m_other = 0;
	}

	explicit EvtData_PhysTrigger_Leave(int triggerID, ActorId other)
		: m_triggerID(triggerID),
		m_other(other)
	{}

	virtual IEvent* VCopy() const
	{
		return new EvtData_PhysTrigger_Leave(m_triggerID, m_other);
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_PhysTrigger_Leave";
	}

	int GetTriggerId(void) const
	{
		return m_triggerID;
	}

	ActorId GetOtherActor(void) const
	{
		return m_other;
	}
};

class EvtData_PhysCollision : public ScriptEvent
{
	ActorId m_ActorA;
	ActorId m_ActorB;
	vec3 m_SumNormalForce;
	vec3 m_SumFrictionForce;
	std::list<vec3> m_CollisionPoints;

public:
	static const EventType sk_EventType;

	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_PhysCollision()
	{
		m_ActorA = 0;
		m_ActorB = 0;
		m_SumNormalForce = vec3(0.0f, 0.0f, 0.0f);
		m_SumFrictionForce = vec3(0.0f, 0.0f, 0.0f);
	}

	explicit EvtData_PhysCollision(ActorId actorA,
		ActorId actorB,
		vec3 sumNormalForce,
		vec3 sumFrictionForce,
		std::list<vec3> collisionPoints)
		: m_ActorA(actorA),
		m_ActorB(actorB),
		m_SumNormalForce(sumNormalForce),
		m_SumFrictionForce(sumFrictionForce),
		m_CollisionPoints(collisionPoints)
	{}

	virtual IEvent* VCopy() const
	{
		return new EvtData_PhysCollision(m_ActorA, m_ActorB, m_SumNormalForce, m_SumFrictionForce, m_CollisionPoints);
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_PhysCollision";
	}

	ActorId GetActorA(void) const
	{
		return m_ActorA;
	}

	ActorId GetActorB(void) const
	{
		return m_ActorB;
	}

	const vec3& GetSumNormalForce(void) const
	{
		return m_SumNormalForce;
	}

	const vec3& GetSumFrictionForce(void) const
	{
		return m_SumFrictionForce;
	}

	const std::list<vec3>& GetCollisionPoints(void) const
	{
		return m_CollisionPoints;
	}

	virtual void VBuildEventData(void);

	EXPORT_FOR_SCRIPT_EVENT(EvtData_PhysCollision);
};


class EvtData_PhysSeparation : public BaseEventData
{
	ActorId m_ActorA;
	ActorId m_ActorB;

public:
	static const EventType sk_EventType;

	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_PhysSeparation()
	{

		m_ActorA = 0;
		m_ActorB = 0;
	}

	explicit EvtData_PhysSeparation(ActorId actorA, ActorId actorB)
		: m_ActorA(actorA)
		, m_ActorB(actorB)
	{}

	virtual IEvent* VCopy() const
	{
		return new EvtData_PhysSeparation(m_ActorA, m_ActorB);
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_PhysSeparation";
	}

	ActorId GetActorA(void) const
	{
		return m_ActorA;
	}

	ActorId GetActorB(void) const
	{
		return m_ActorB;
	}
};
