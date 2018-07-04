#pragma once
#include <pch.h>
class Actor;

class ActorComponent: public IComponent
{
	friend class ActorFactory;
protected:
	IActor* m_pOwner;
public:
	ActorComponent(Context* pContext);			
	virtual IActor* VGetOwner() { return m_pOwner; }
	
	virtual ComponentId VGetId(void) const final{ return GetIdFromName(VGetName()); }

	static ComponentId GetIdFromName(const char* componentStr)
	{
		void* rawId = HashedString::hash_name(componentStr);
		return reinterpret_cast<ComponentId>(rawId);
	}

	

private:
	void SetOwner(IActor* pOwner) { m_pOwner = pOwner; }
};