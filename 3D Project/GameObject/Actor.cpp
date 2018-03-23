#include "Actor.h"
#include <pch.h>


Context* Actor::m_Context = nullptr;
Actor::Actor(ActorId id) :m_id(id), m_pParent(nullptr),m_State(AS_NORMAL)
{

}

Actor::~Actor()
{

}

bool Actor::Init(const tinyxml2::XMLElement* pData)
{
	const char* tag = pData->Attribute("type");
	const char* name = pData->Attribute("name");
	int state = pData->Int64Attribute("State",AS_NORMAL);

	m_Tag = tag;
	m_Name = name;
	m_State = (ActorState)state;
	return 1;
}

void Actor::PostInit(void)
{
	for (ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->VPostInit();
	}
	m_TransformComponent->VPostInit();
}

void Actor::Destroy(void)
{
}

HRESULT Actor::VOnUpdate(Scene *pScene, float deltaMs)
{
	if (m_State == AS_BLOCK) return S_OK;

	for (ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->VUpdate(deltaMs);
	}

	
	for(ActorList::iterator i = m_Children.begin();i!= m_Children.end();i++)
	{
		(*i)->VOnUpdate(pScene, deltaMs);
	}
	return S_OK;

	
}

HRESULT Actor::VPostUpdate(Scene *pScene)
{
	for (ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->VPostUpdate();
	}

	for (ActorList::iterator i = m_Children.begin(); i != m_Children.end(); i++)
	{
		(*i)->VPostUpdate(pScene);
	}
	return S_OK;
}

void Actor::VSetTransform(const mat4& toWorld)
{
	m_TransformComponent->SetTransform(toWorld);
}

const mat4& Actor::VGetTransform()const 
{
	return m_TransformComponent->GetTransform();
}

mat4 Actor::VGetGlobalTransform()
{
	
	mat4 transform = m_TransformComponent->GetTransform();
	if (m_pParent)
		transform = m_pParent->VGetGlobalTransform()*transform;

	return transform;
}

HRESULT Actor::VPreRender(Scene * pScene)
{
	return S_OK;
}

bool Actor::VIsVisible(Scene * pScene) const
{
	return true;
}

TransformComponent * Actor::GetTransform()
{
	return m_TransformComponent.get();
}




void Actor::AddComponent(ActorComponent * pComponent)
{
	std::pair<ActorComponents::iterator, bool> success = m_components.insert(std::make_pair(pComponent->VGetId(), pComponent));
	//GCC_ASSERT(success.second);
}

void Actor::SetTransformComponent(TransformComponent * pTC)
{
	m_TransformComponent = std::unique_ptr<TransformComponent>(pTC);
}

HRESULT Actor::VRender(Scene * pScene)
{	
	return S_OK;	
}

void Actor::VSetState(ActorState state)
{
	m_State = state;
}

Actor::ActorState Actor::VGetState()
{
	return m_State;
}

HRESULT Actor::VRenderChildren(Scene * pScene)
{
	
	// Iterate through the children....	
	for(ActorList::iterator i = m_Children.begin(); i!=m_Children.end();i++)
	{
		if ((*i)->VPreRender(pScene) == S_OK)
		{

			// Don't render this node if you can't see it
			if ((*i)->VIsVisible(pScene)) (*i)->VRender(pScene);

			(*i)->VRenderChildren(pScene);
		}
		(*i)->VPostRender(pScene);
	}	
	return S_OK;
}

HRESULT Actor::VPostRender(Scene * pScene)
{
	return E_NOTIMPL;
}

bool Actor::VAddChild(std::unique_ptr<Actor> kid)
{
	Actor* p = kid.get();
	m_Children.push_back(std::move(kid));
	p->m_pParent = this;
	return true;
}

bool Actor::VRemoveChild(ActorId id)
{
	for (ActorList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
	{
		if ((*it)->GetId() == id)
		{
			m_Children.erase(it);
			return true;
		}
	}

	return false;
}

Actor * Actor::VGetChild(int index)
{
	assert(index >= 0 && index < m_Children.size());
	return m_Children[index].get();
}

Actor * Actor::VGetChild(const string & name)
{
	for (auto& el = m_Children.begin(); el != m_Children.end(); el++)
		if ((*el)->VGetName() == name) return (*el).get();

	return nullptr;
}

Actor * Actor::VGetParent()
{
	return m_pParent;
}


// accessors

ActorId Actor::GetId(void) const { return m_id; }

glm::vec3 Actor::GetPosition()
{
	return m_TransformComponent->GetPosition();
}


