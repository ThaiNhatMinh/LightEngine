#pragma once
#include "pch.h"
class Actor;
class ActorComponent;

//class TiXmlElement;

class Actor//: public ISceneNode
{
	friend class ActorFactory;
public:
	typedef std::vector<std::unique_ptr<Actor>> ActorList;
	typedef std::map<ComponentId, std::unique_ptr<ActorComponent>> ActorComponents;

	enum ActorState
	{
		AS_NORMAL,
		AS_BLOCK,
	};
protected:
	ActorList				m_Children;
	Actor*					m_pParent;
	string					m_Name;
	string					m_Tag;
	ActorState				m_State;
	std::unique_ptr<TransformComponent>		m_TransformComponent;
protected:
	ActorId m_id;					// unique id for the actor
	ActorComponents m_components;	// all components this actor has

public:
	Actor(ActorId id);
	virtual ~Actor();

	virtual bool		Init(const tinyxml2::XMLElement* pData);
	virtual void		PostInit(void);
	virtual void		Destroy(void);

	virtual void		VSetName(string name) { m_Name = name; }
	virtual string		VGetName() {	return m_Name;	};
	virtual void		VSetTag(string tag) { m_Tag=tag; };
	virtual string		VGetTag() { return m_Tag; };
	virtual void		VSetTransform(const mat4& toWorld);
	virtual mat4		VGetTransform();
	virtual mat4		VGetGlobalTransform();
	virtual HRESULT		VOnUpdate(Scene *, float elapsedMs);
	virtual HRESULT		VPostUpdate(Scene *);
	virtual HRESULT		VPreRender(Scene *pScene);
	virtual bool		VIsVisible(Scene *pScene) const ;
	virtual HRESULT		VRenderChildren(Scene *pScene);
	virtual HRESULT		VPostRender(Scene *pScene);
	virtual HRESULT		VRender(Scene *pScene);
	virtual void		VSetState(ActorState state);
	virtual ActorState	VGetState();
	virtual bool		VAddChild(std::unique_ptr<Actor> kid);
	virtual bool		VRemoveChild(ActorId id);
	virtual Actor*		VGetChild(int index);
	virtual Actor*		VGetChild(const string& name);
	virtual Actor*		VGetParent();
	// accessors
	ActorId GetId(void) const { return m_id; }
	template<class ComponentType>ComponentType* GetComponent(ComponentId id);
	template<class ComponentType>ComponentType* GetComponent(const char*  name)const ;

	template<class ComponentType>ComponentType* RemoveComponent(const char*  name);
	const ActorComponents* GetComponents() { return &m_components; }
	TransformComponent* GetTransform();

	static Context* m_Context;
protected:

	void AddComponent(ActorComponent* pComponent);
	void SetTransformComponent(TransformComponent* pTC);
	
	
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
inline ComponentType * Actor::GetComponent(const char * name)const
{
	ComponentId id = ActorComponent::GetIdFromName(name);
	ActorComponents::const_iterator findIt = m_components.find(id);
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

template<class ComponentType>ComponentType* Actor::RemoveComponent(const char*  name)
{
	ComponentId id = ActorComponent::GetIdFromName(name);
	ActorComponents::iterator findIt = m_components.find(id);
	if (findIt != m_components.end())
	{
		ActorComponent* pBase(findIt->second.release());
		m_components.erase(id);
		ComponentType* pWeakSub = static_cast<ComponentType*>(pBase);
		return pWeakSub;  // return the weak pointer
	}
	else
	{
		return nullptr;
	}
}