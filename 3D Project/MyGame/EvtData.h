#pragma once

class EvtData_PlayerWpData : public IEvent
{

public:
	EvtData_PlayerWpData(const std::vector<WeaponResource>& v):wr(v)
	{
	};
	static const EventType sk_EventType;
	virtual const EventType& VGetEventType(void) const 
	{
		return sk_EventType;
	};
	virtual void VSerialize(std::ostrstream& out) const {};
	virtual void VDeserialize(std::istrstream& in) {};
	virtual IEvent* VCopy(void) const
	{
		return new EvtData_PlayerWpData(wr);
	};
	virtual const char* GetName(void) const
	{
		return "EvtData_PlayerWpData";
	};
	
	const vector<WeaponResource>& wr;
};

class EvtData_PlayerCharData : public IEvent
{

public:
	EvtData_PlayerCharData(const std::vector<CharacterResource>& v) :wr(v)
	{
	};
	static const EventType sk_EventType;
	virtual const EventType& VGetEventType(void) const
	{
		return sk_EventType;
	};
	virtual void VSerialize(std::ostrstream& out) const {};
	virtual void VDeserialize(std::istrstream& in) {};
	virtual IEvent* VCopy(void) const
	{
		return new EvtData_PlayerCharData(wr);
	};
	virtual const char* GetName(void) const
	{
		return "EvtData_PlayerCharData";
	};

	const vector<CharacterResource>& wr;
};

class EvtData_RequestCreateWeapon :public IEvent
{
public:
	EvtData_RequestCreateWeapon(Actor* p,const char* n) :Parent(p), WPName(n)
	{
	};
	static const EventType sk_EventType;
	virtual const EventType& VGetEventType(void) const
	{
		return sk_EventType;
	};
	virtual void VSerialize(std::ostrstream& out) const {};
	virtual void VDeserialize(std::istrstream& in) {};
	virtual IEvent* VCopy(void) const
	{
		return new EvtData_RequestCreateWeapon(Parent, WPName);
	};
	virtual const char* GetName(void) const
	{
		return "EvtData_RequestCreateActor";
	};
	const char* WPName;
	Actor* Parent;
};