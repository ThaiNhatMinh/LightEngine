#pragma once
#include <pch.h>

class PhysicCollisionData
{
public:
	vec3 pos;
	vec3 normal;
	PhysicCollisionData(vec3 p, vec3 n) :pos(p), normal(n) {}

};

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

class EvtData_PhysCollisionStart : public BaseEventData
{
	Actor* m_ActorA;
	Actor* m_ActorB;
	vec3 m_SumNormalForce;
	vec3 m_SumFrictionForce;
	vector<PhysicCollisionData> m_CollisionPoints;

public:
	static const EventType sk_EventType;

	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_PhysCollisionStart()
	{
		m_ActorA = 0;
		m_ActorB = 0;
		m_SumNormalForce = vec3(0.0f, 0.0f, 0.0f);
		m_SumFrictionForce = vec3(0.0f, 0.0f, 0.0f);
	}

	explicit EvtData_PhysCollisionStart(Actor* actorA,
		Actor* actorB,
		vec3 sumNormalForce,
		vec3 sumFrictionForce,
		vector<PhysicCollisionData> collisionPoints)
		: m_ActorA(actorA),
		m_ActorB(actorB),
		m_SumNormalForce(sumNormalForce),
		m_SumFrictionForce(sumFrictionForce),
		m_CollisionPoints(collisionPoints)
	{}

	virtual IEvent* VCopy() const
	{
		return new EvtData_PhysCollisionStart(m_ActorA, m_ActorB, m_SumNormalForce, m_SumFrictionForce, m_CollisionPoints);
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_PhysCollisionStart";
	}

	Actor* GetActorA(void) const
	{
		return m_ActorA;
	}

	Actor* GetActorB(void) const
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

	const vector<PhysicCollisionData>& GetCollisionPoints(void) const
	{
		return m_CollisionPoints;
	}

};


class EvtData_PhysOnCollision : public BaseEventData
{
	Actor* m_ActorA;
	Actor* m_ActorB;
	
	vec3 m_SumNormalForce;
	vec3 m_SumFrictionForce;
	vector<PhysicCollisionData> m_CollisionData;

public:
	static const EventType sk_EventType;

	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_PhysOnCollision()
	{
		m_ActorA = 0;
		m_ActorB = 0;
		m_SumNormalForce = vec3(0.0f, 0.0f, 0.0f);
		m_SumFrictionForce = vec3(0.0f, 0.0f, 0.0f);
	}

	explicit EvtData_PhysOnCollision(Actor* actorA,
		Actor* actorB,
		vec3 sumNormalForce,
		vec3 sumFrictionForce,
		vector<PhysicCollisionData> collisionPoints)
		: m_ActorA(actorA),
		m_ActorB(actorB),
		m_SumNormalForce(sumNormalForce),
		m_SumFrictionForce(sumFrictionForce),
		m_CollisionData(collisionPoints)
	{}

	virtual IEvent* VCopy() const
	{
		return new EvtData_PhysOnCollision(m_ActorA, m_ActorB, m_SumNormalForce, m_SumFrictionForce, m_CollisionData);
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_PhysOnCollision";
	}

	Actor* GetActorA(void) const
	{
		return m_ActorA;
	}

	Actor* GetActorB(void) const
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

	const vector<PhysicCollisionData>& GetCollisionPoints(void) const
	{
		return m_CollisionData;
	}

};

class EvtData_PhysCollisionEnd : public BaseEventData
{
	ActorId m_ActorA;
	ActorId m_ActorB;

public:
	static const EventType sk_EventType;

	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_PhysCollisionEnd()
	{

		m_ActorA = 0;
		m_ActorB = 0;
	}

	explicit EvtData_PhysCollisionEnd(ActorId actorA, ActorId actorB)
		: m_ActorA(actorA)
		, m_ActorB(actorB)
	{}

	virtual IEvent* VCopy() const
	{
		return new EvtData_PhysCollisionEnd(m_ActorA, m_ActorB);
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_PhysCollisionEnd";
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

class EvtData_PhysPreStep : public BaseEventData
{
	float m_fTimeStep;

public:
	static const EventType sk_EventType;

	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_PhysPreStep()
	{
		m_fTimeStep = 0;
	}

	explicit EvtData_PhysPreStep(float timestep)
		: m_fTimeStep(timestep)
	{}

	IEvent* VCopy() const
	{
		return new EvtData_PhysPreStep(m_fTimeStep);
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_PhysPreStep";
	}
	float GetTimeStep()const { return m_fTimeStep; };
};


class EvtData_PhysPostStep : public BaseEventData
{
	float m_fTimeStep;

public:
	static const EventType sk_EventType;

	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_PhysPostStep()
	{
		m_fTimeStep = 0;
	}

	explicit EvtData_PhysPostStep(float timestep)
		: m_fTimeStep(timestep)
	{}

	IEvent* VCopy() const
	{
		return new EvtData_PhysPreStep(m_fTimeStep);
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_PhysPostStep";
	}
	float GetTimeStep() const{ return m_fTimeStep; };
};