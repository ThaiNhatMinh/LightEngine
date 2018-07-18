#pragma once
#include "IComponent.h"
class Scene;
namespace Light
{
	class IActor
	{
	public:

		virtual ~IActor() = default;

		virtual bool		Init(const tinyxml2::XMLElement* pData) = 0;
		virtual void		PostInit(void) = 0;
		virtual void		Destroy(void) = 0;

		virtual void		VSetName(std::string name) = 0;
		virtual string		VGetName() = 0;
		virtual void		VSetTag(string tag) = 0;
		virtual string		VGetTag() = 0;
		virtual mat4		VGetGlobalTransform() = 0;
		virtual HRESULT		VOnUpdate(Scene *, float elapsedMs) = 0;
		virtual HRESULT		VPostUpdate(Scene *) = 0;
		virtual HRESULT		VPreRender(Scene *pScene) = 0;
		virtual bool		VIsVisible(Scene *pScene) const = 0;
		virtual HRESULT		VRenderChildren(Scene *pScene) = 0;
		virtual HRESULT		VPostRender(Scene *pScene) = 0;
		virtual HRESULT		VRender(Scene *pScene) = 0;
		virtual bool		VAddChild(IActor* kid) = 0;
		virtual bool		VRemoveChild(ActorId id) = 0;
		virtual IActor*		VGetChild(int index) = 0;
		virtual IActor*		VGetChild(const string& name) = 0;
		virtual IActor*		VGetParent() = 0;
		virtual ActorId		VGetId(void)const = 0;

		virtual bool		VAddComponent(IComponent* pComponent) = 0;
		virtual IComponent* VGetComponent(ComponentId id) = 0;
		virtual bool		VRemoveComponent(ComponentId id) = 0;

		template<class ComponentType>ComponentType* GetComponent();
		template<class ComponentType>bool RemoveComponent();
	};

	template<class ComponentType>
	inline ComponentType* IActor::GetComponent()
	{
		return static_cast<ComponentType*>(VGetComponent(typeid(ComponentType*).hash_code()));
	}
	template<class ComponentType>
	inline bool IActor::RemoveComponent()
	{
		return VRemoveComponent(typeid(ComponentType*).hash_code());
	}
}