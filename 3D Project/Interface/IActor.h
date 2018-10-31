#pragma once
#include "IComponent.h"
#include "IScript.h"
#include "..\typedef.h"
#include <string>

class IScene;
namespace Light
{
	struct ActorProperty
	{

	};
	class IActor
	{
	public:
		
	public:

		virtual ~IActor() = default;

		virtual bool		Init(const tinyxml2::XMLElement* pData) = 0;
		virtual void		PostInit(void) = 0;
		virtual void		Destroy(void) = 0;

		virtual void		VSetName(std::string name) = 0;
		virtual std::string	VGetName() = 0;
		virtual void		VSetTag(std::string tag) = 0;
		virtual std::string	VGetTag() = 0;
		virtual glm::mat4	VGetGlobalTransform() = 0;
		virtual HRESULT		VOnUpdate(IScene *, float elapsedMs) = 0;
		virtual HRESULT		VPostUpdate(IScene *) = 0;
		virtual void		VPreRender(render::Material::MatrixParam& param) = 0;
		virtual bool		VIsVisible(IScene *pScene) const = 0;
		virtual bool		VAddChild(IActor* kid) = 0;
		virtual bool		VRemoveChild(ActorId id) = 0;
		virtual IActor*		VGetChild(std::size_t index) = 0;
		virtual IActor*		VGetChild(const std::string& name) = 0;
		virtual IActor*		VGetParent() = 0;
		virtual ActorId		VGetId(void)const = 0;

		virtual bool		VAddComponent(IComponent* pComponent) = 0;
		virtual IComponent* VGetComponent(ComponentType id) = 0;
		virtual bool		VRemoveComponent(ComponentType id) = 0;

		template<class ComponentType>ComponentType* GetComponent();
		template<class ComponentType>bool RemoveComponent();
		// Current using for test.
		// Script can using Lua instead of C++
		virtual void		VSetScript(IScript* pScript) = 0;
	};

	template<class ComponentType>
	inline ComponentType* IActor::GetComponent()
	{
		return static_cast<ComponentType*>(VGetComponent(typeid(ComponentType).hash_code()));
	}
	template<class ComponentType>
	inline bool IActor::RemoveComponent()
	{
		return VRemoveComponent(typeid(ComponentType*).hash_code());
	}
}