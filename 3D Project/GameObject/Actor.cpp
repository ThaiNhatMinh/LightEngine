#include "pch.h"

Actor::Actor(ActorId id) :m_id(id), m_pParent(nullptr)
{

}

Actor::~Actor()
{

}

bool Actor::Init(const tinyxml2::XMLElement* pData)
{
	const char* tag = pData->Attribute("type");
	const char* name = pData->Attribute("name");


	m_Tag = tag;
	m_Name = name;

	return 1;
}

void Actor::PostInit(void)
{
	for (ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->VPostInit();
	}
}

void Actor::Destroy(void)
{
}

HRESULT Actor::VOnUpdate(Scene *pScene, float deltaMs)
{
	for (ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->VUpdate(deltaMs);
	}

	ActorList::iterator i = m_Children.begin();
	ActorList::iterator end = m_Children.end();

	while (i != end)
	{
		(*i)->VOnUpdate(pScene, deltaMs);
		++i;
	}
	return S_OK;

	
}

void Actor::VSetTransform(const mat4 * toWorld)
{
	m_TransformComponent->SetTransform(*toWorld);
}

mat4 Actor::VGetTransform()
{
	mat4 transform = m_TransformComponent->GetTransform();
	return transform;
}

mat4 Actor::VGetGlobalTransform()
{
	
	mat4 transform = m_TransformComponent->GetTransform();
	if (m_pParent)
		transform = transform*m_pParent->VGetGlobalTransform();

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

HRESULT Actor::VRenderChildren(Scene * pScene)
{
	// Iterate through the children....
	ActorList::iterator i = m_Children.begin();
	ActorList::iterator end = m_Children.end();
	
	while (i != end)
	{
		if ((*i)->VPreRender(pScene) == S_OK)
		{

			// Don't render this node if you can't see it
			if ((*i)->VIsVisible(pScene)) (*i)->VRender(pScene);

			(*i)->VRenderChildren(pScene);
		}
		(*i)->VPostRender(pScene);
		++i;
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

Actor * Actor::VGetParent()
{
	return m_pParent;
}
