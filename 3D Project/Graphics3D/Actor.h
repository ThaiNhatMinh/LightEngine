#pragma once
#include <pch.h>
#include "..\Interface\IActor.h"
#include "..\Interface\IComponent.h"
#include "..\Interface\IEventManager.h"
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
		std::unique_ptr<IScript>m_pScript;
		IEventManager*			m_pEventManager;

		std::vector<util::Updatable*> m_ObjectUpdate;
		std::vector<util::PreRenderable*> m_ObjectPreRender;
		bool					m_bIsStatic;
	public:
		Actor(IContext* pContext,ActorId id);
		virtual ~Actor();

		virtual bool		Init(const tinyxml2::XMLElement* pData)override;
		virtual void		PostInit(void)override;
		virtual void		Destroy(void)override;

		virtual void		VSetName(string name)override { m_Name = name; }
		virtual string		VGetName() override { return m_Name; };
		virtual void		VSetTag(string tag)override { m_Tag = tag; };
		virtual string		VGetTag()override { return m_Tag; };
		virtual mat4		VGetGlobalTransform()override;
		virtual HRESULT		VOnUpdate(IScene *, float elapsedMs)override;
		virtual HRESULT		VPostUpdate(IScene *)override;
		virtual void		VPreRender(render::Material::MatrixParam& param)override;
		virtual bool		VIsVisible(IScene *pScene) const override;
		virtual bool		VAddChild(IActor* kid)override;
		virtual bool		VRemoveChild(ActorId id)override;
		virtual IActor*		VGetChild(std::size_t index)override;
		virtual IActor*		VGetChild(const string& name)override;
		virtual IActor*		VGetParent()override; 
		virtual ActorId		VGetId(void) const override;
		virtual bool		VAddComponent(IComponent* pComponent)override;
		virtual IComponent* VGetComponent(ComponentType id)override;
		virtual bool		VRemoveComponent(ComponentType id)override;
		virtual void		VSetScript(IScript* pScript)override;
		

	};

	
}