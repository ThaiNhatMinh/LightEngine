//#pragma once
//#include <pch.h>
//class Actor;
//
//class ActorComponent: public IComponent
//{
//	friend class ActorFactory;
//protected:
//	IActor* m_pOwner;
//public:
//	virtual bool VInit(Context* pContext, const tinyxml2::XMLElement* pData) { return true; }
//	virtual tinyxml2::XMLElement*	VGenerateXml(tinyxml2::XMLDocument*p) { return nullptr; };
//	virtual void					VPostInit(void) {};
//	virtual void					VUpdate(float dt) {};
//	virtual void					VPostUpdate() {};
//	virtual IActor*					VGetOwner() { return m_pOwner; }
//	virtual ComponentId				VGetId(void) const final{ return GetIdFromName(VGetName()); }
//
//	static ComponentId GetIdFromName(const char* componentStr)
//	{
//		void* rawId = HashedString::hash_name(componentStr);
//		return reinterpret_cast<ComponentId>(rawId);
//	}
//
//	
//
//private:
//	void SetOwner(IActor* pOwner) { m_pOwner = pOwner; }
//};