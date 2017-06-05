#pragma once
#include "..\pch.h"
class Actor;

class ActorComponent
{
	friend class ActorFactory;
protected:
	Actor* m_pOwner;
public:
	virtual ~ActorComponent(void) {};
	// These functions are meant to be overridden by the implementation classes of the components.
	virtual bool VInit(tinyxml2::XMLElement* pData) = 0;
	virtual void VPostInit(void) { }
	virtual void VUpdate(int deltaMs) { }
	virtual void VOnChanged(void) { }				// [mrmike] - added post-pres
													// for the editor
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p) = 0;

	// This function should be overridden by the interface class.
	virtual ComponentId VGetId(void) const { return GetIdFromName(VGetName()); }
	virtual const char *VGetName() const = 0;
	static ComponentId GetIdFromName(const char* componentStr)
	{
		void* rawId = HashedString::hash_name(componentStr);
		return reinterpret_cast<ComponentId>(rawId);
	}

private:
	void SetOwner(Actor* pOwner) { m_pOwner = pOwner; }
};