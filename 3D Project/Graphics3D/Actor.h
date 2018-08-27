#pragma once
#include <pch.h>
#include "..\Interface\IActor.h"
#include "..\Interface\IComponent.h"
namespace Light 
{
	class Actor : public IActor
	{
		friend class ActorFactory;
	public:
		typedef std::vector<std::unique_ptr<IActor>> ActorList;
		typedef std::map<ComponentType, std::unique_ptr<IComponent>> ActorComponents;


	protected:
		ActorList				m_Children;
		IActor*					m_pParent;
		string					m_Name;
		string					m_Tag;
		std::unique_ptr<ITransformComponent>		m_TransformComponent;
		ActorId					m_id;					// unique id for the actor
		ActorComponents			m_components;	// all components this actor has

		glm::mat4				m_WorldTransform;
	public:
		Actor(ActorId id);
		virtual ~Actor();

		virtual bool		Init(const tinyxml2::XMLElement* pData)override;
		virtual void		PostInit(void)override;
		virtual void		Destroy(void)override;

		virtual void		VSetName(string name)override { m_Name = name; }
		virtual string		VGetName() override { return m_Name; };
		virtual void		VSetTag(string tag)override { m_Tag = tag; };
		virtual string		VGetTag()override { return m_Tag; };
		virtual mat4		VGetGlobalTransform()override;
		virtual HRESULT		VOnUpdate(Scene *, float elapsedMs)override;
		virtual HRESULT		VPostUpdate(Scene *)override;
		virtual bool		VIsVisible(Scene *pScene) const override;
		virtual bool		VAddChild(IActor* kid)override;
		virtual bool		VRemoveChild(ActorId id)override;
		virtual IActor*		VGetChild(std::size_t index)override;
		virtual IActor*		VGetChild(const string& name)override;
		virtual IActor*		VGetParent()override; 
		virtual ActorId		VGetId(void) const override;
		virtual bool		VAddComponent(IComponent* pComponent)override;
		virtual IComponent* VGetComponent(ComponentType id)override;
		virtual bool		VRemoveComponent(ComponentType id)override;
		//template<class ComponentType>ComponentType* GetComponent(ComponentId id);
		//template<class ComponentType>ComponentType* GetComponent(const char*  name)const;

		//template<class ComponentType>ComponentType* RemoveComponent(const char*  name);
		//const ActorComponents*						GetComponents() { return &m_components; }
		//TransformComponent*							GetTransform();

	};

	//template<class ComponentType>
	//inline ComponentType * Actor::GetComponent(ComponentId id)
	//{
	//	ActorComponents::iterator findIt = m_components.find(id);
	//	if (findIt != m_components.end())
	//	{
	//		ActorComponent* pBase(findIt->second.get());
	//		ComponentType* pWeakSub = static_cast<ComponentType*>(pBase);
	//		return pWeakSub;  // return the weak pointer
	//	}
	//	else
	//	{
	//		return nullptr;
	//	}
	//}

	//template<class ComponentType>
	//inline ComponentType * Actor::GetComponent(const char * name)const
	//{
	//	ComponentId id = ActorComponent::GetIdFromName(name);
	//	ActorComponents::const_iterator findIt = m_components.find(id);
	//	if (findIt != m_components.end())
	//	{
	//		ActorComponent* pBase(findIt->second.get());
	//		ComponentType* pWeakSub = static_cast<ComponentType*>(pBase);
	//		return pWeakSub;  // return the weak pointer
	//	}
	//	else
	//	{
	//		return nullptr;
	//	}
	//}

	//template<class ComponentType>ComponentType* Actor::RemoveComponent(const char*  name)
	//{
	//	ComponentId id = ActorComponent::GetIdFromName(name);
	//	ActorComponents::iterator findIt = m_components.find(id);
	//	if (findIt != m_components.end())
	//	{
	//		ActorComponent* pBase(findIt->second.release());
	//		m_components.erase(id);
	//		ComponentType* pWeakSub = static_cast<ComponentType*>(pBase);
	//		return pWeakSub;  // return the weak pointer
	//	}
	//	else
	//	{
	//		return nullptr;
	//	}
	//}
}