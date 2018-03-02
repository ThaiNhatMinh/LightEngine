

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
	std::string File;
	const char* WPName;
	Actor* Parent;
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