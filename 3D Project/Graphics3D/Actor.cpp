#include "Actor.h"
#include <pch.h>
namespace Light
{
	Actor::Actor(ActorId id) :m_id(id), m_pParent(nullptr), m_pScript(DEBUG_NEW NullScript()) // default is nullscript
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
		// Update m_WorldTrasform
		VGetGlobalTransform();

		for (ActorList::iterator i = m_Children.begin(); i != m_Children.end(); i++)
			(*i)->PostInit();

		m_pScript->PostInit();
	}

	void Actor::Destroy(void)
	{
	}

	HRESULT Actor::VOnUpdate(Scene *pScene, float deltaMs)
	{
		m_WorldTransform = m_TransformComponent->transform;
		if (m_pParent)
			m_WorldTransform = m_pParent->VGetGlobalTransform()*m_WorldTransform;
		// Script update first
		m_pScript->Update(deltaMs);


		for (ActorList::iterator i = m_Children.begin(); i != m_Children.end(); i++)
		{
			(*i)->VOnUpdate(pScene, deltaMs);
		}
		return S_OK;


	}

	HRESULT Actor::VPostUpdate(Scene *pScene)
	{


		for (ActorList::iterator i = m_Children.begin(); i != m_Children.end(); i++)
		{
			(*i)->VPostUpdate(pScene);
		}
		return S_OK;
	}

	mat4 Actor::VGetGlobalTransform()
	{

	

		return m_WorldTransform;
	}


	bool Actor::VIsVisible(Scene * pScene) const
	{
		return true;
	}

	




	bool Actor::VAddComponent(IComponent * pComponent)
	{
		if (pComponent->GetType()==ITransformComponent::StaticType)
		{
			m_TransformComponent = std::unique_ptr<ITransformComponent>(static_cast<ITransformComponent*>(pComponent));
			return true;
		}
		else if (pComponent->GetType() == ICameraComponent::StaticType)
		{
			auto pTemp = static_cast<ICameraComponent*>(pComponent);
			pTemp->m_GlobalTransform = &m_WorldTransform;
		}

		std::pair<ActorComponents::iterator, bool> success = m_components.insert(std::make_pair(pComponent->GetType(), pComponent));
		return success.second;
	}

	IComponent * Actor::VGetComponent(ComponentType id)
	{
		if (id == ITransformComponent::StaticType) return m_TransformComponent.get();


		ActorComponents::iterator findIt = m_components.find(id);
		if (findIt != m_components.end())
		{
			IComponent* pBase(findIt->second.get());
			
			return pBase;
		}
		else
		{
			return nullptr;
		}
	}

	bool Actor::VRemoveComponent(ComponentType id)
	{
		ActorComponents::iterator findIt = m_components.find(id);
		if (findIt != m_components.end())
		{
			m_components.erase(findIt);
			return true;
		}
		
		
		return false;
		
	}

	void Actor::VSetScript(IScript * pScript)
	{
		m_pScript.release();
		m_pScript = std::unique_ptr<IScript>(pScript);
	}

	

	bool Actor::VAddChild(IActor* kid)
	{
		Actor* p = static_cast<Actor*>(kid);
		m_Children.push_back(std::unique_ptr<IActor>(kid));
		p->m_pParent = this;
		return true;
	}

	bool Actor::VRemoveChild(ActorId id)
	{
		for (ActorList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->VGetId() == id)
			{
				m_Children.erase(it);
				return true;
			}
		}

		return false;
	}

	IActor * Actor::VGetChild(std::size_t index)
	{
		assert(index >= 0 && index < m_Children.size());
		return m_Children[index].get();
	}

	IActor * Actor::VGetChild(const string & name)
	{
		for (auto& el = m_Children.begin(); el != m_Children.end(); el++)
			if ((*el)->VGetName() == name) return (*el).get();

		return nullptr;
	}

	IActor * Actor::VGetParent()
	{
		return m_pParent;
	}

	ActorId Actor::VGetId(void) const { return m_id; }


}
