#pragma once
#include "pch.h"
class Actor;
class ActorComponent;

//class TiXmlElement;

class Actor: public ISceneNode
{
	friend class ActorFactory;
public:
	typedef std::vector<std::unique_ptr<ISceneNode>> ActorList;
	typedef std::map<ComponentId, std::unique_ptr<ActorComponent>> ActorComponents;
protected:
	ActorList				m_Children;
	Actor*					m_pParent;
	string					m_Name;
	string					m_Tag;

private:
	ActorId m_id;					// unique id for the actor
	ActorComponents m_components;	// all components this actor has

public:
	Actor(ActorId id);
	~Actor();

	virtual bool Init(const tinyxml2::XMLElement* pData);
	virtual void PostInit(void);
	virtual void Destroy(void);

	virtual void VSetName(string name) { m_Name = name; }
	virtual string VGetName() {	return m_Name;	};
	virtual void VSetTransform(const mat4 *toWorld);
	virtual mat4 VGetTransform();
	virtual mat4 VGetGlobalTransform();
	virtual HRESULT VOnUpdate(Scene *, float elapsedMs);

	virtual HRESULT VPreRender(Scene *pScene);
	virtual bool VIsVisible(Scene *pScene) const { return true; };
	virtual HRESULT VRenderChildren(Scene *pScene);
	virtual HRESULT VPostRender(Scene *pScene);
	virtual HRESULT VRender(Scene *pScene);

	virtual bool VAddChild(std::unique_ptr<Actor> kid);
	virtual bool VRemoveChild(ActorId id);
	virtual Actor* VGetParent();
	// accessors
	ActorId GetId(void) const { return m_id; }
	template<class ComponentType>ComponentType* GetComponent(ComponentId id);
	template<class ComponentType>ComponentType* GetComponent(const char*  name);

	const ActorComponents* GetComponents() { return &m_components; }
protected:

	void AddComponent(ActorComponent* pComponent);

	
	
};

template<class ComponentType>
inline ComponentType * Actor::GetComponent(ComponentId id)
{
	ActorComponents::iterator findIt = m_components.find(id);
	if (findIt != m_components.end())
	{
		ActorComponent* pBase(findIt->second.get());
		ComponentType* pWeakSub = static_cast<ComponentType*>(pBase);
		return pWeakSub;  // return the weak pointer
	}
	else
	{
		return nullptr;
	}
}

template<class ComponentType>
inline ComponentType * Actor::GetComponent(const char * name)
{
	ComponentId id = ActorComponent::GetIdFromName(name);
	ActorComponents::iterator findIt = m_components.find(id);
	if (findIt != m_components.end())
	{
		ActorComponent* pBase(findIt->second.get());
		ComponentType* pWeakSub = static_cast<ComponentType*>(pBase);
		return pWeakSub;  // return the weak pointer
	}
	else
	{
		return nullptr;
	}
}

