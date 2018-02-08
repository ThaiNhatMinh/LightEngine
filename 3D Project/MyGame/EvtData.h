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
	string File;
	const char* WPName;
	Actor* Parent;
};

class EvtRequestCreateSprite :public IEvent
{
public:
	EvtRequestCreateSprite(const string& file, const vec3& pos);;

	const vec3& GetPos();
	Actor* GetParent();
	void SetParent(Actor* p);
	void SetLoop(bool loop);
	bool isLoop();
	const string& GetFile();
	
public:
	EVENT_DEFINE(EvtRequestCreateSprite)
private:
	bool Loop;
	string File;
	Actor* Parent;
	vec3 Pos;
};

class EvtTakeDamage : public IEvent
{
public:
	EvtTakeDamage(Creature* attacker, Creature* victim, int damage);
	Creature* GetAttacker();
	Creature* GetVictim();
	int GetDamage();
public:
	EVENT_DEFINE(EvtTakeDamage)
private:
	Creature* Attacker;
	Creature* Victim;
	int Damage;
};