#pragma once

#include <pch.h>



class EvtData_SetAnimation : public BaseEventData
{
	ActorId m_id;
	GLuint m_AnimID;
	bool bDefaultAnim;
public:
	static const EventType sk_EventType;

	virtual const EventType& VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_SetAnimation(void):m_id(0),m_AnimID(0),bDefaultAnim(0)
	{
		
	}

	EvtData_SetAnimation(ActorId id, const GLuint& anim,bool default=false)
		: m_id(id), m_AnimID(anim),bDefaultAnim(default)
	{
		//
	}

	virtual IEvent* VCopy() const
	{
		return new EvtData_SetAnimation(m_id, m_AnimID);
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_SetAnimation";
	}

	ActorId GetId(void) const
	{
		return m_id;
	}
	GLuint GetAnimation()const
	{
		return m_AnimID;
	}

	bool isDefault()const { return bDefaultAnim; }
};

class EvtData_AnimationString : public BaseEventData
{
	ActorId m_id;
	string m_String;
public:
	static const EventType sk_EventType;

	virtual const EventType& VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_AnimationString(void) :m_id(0)
	{

	}

	EvtData_AnimationString(ActorId id, const string& str)
		: m_id(id), m_String(str)
	{
		//
	}

	virtual IEvent* VCopy() const
	{
		return new EvtData_AnimationString(m_id, m_String);
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_AnimationString";
	}

	ActorId GetId(void) const
	{
		return m_id;
	}
	string GetString()const
	{
		return m_String;
	}
};
	