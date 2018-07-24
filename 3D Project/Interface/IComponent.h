#pragma once

#include <glm\mat4x4.hpp>
#include <Utilities\Utility.h>
#include "..\Graphics3D\ModelRender.h"
namespace Light 
{
	class IActor;
	class IComponent: public util::Serialization
	{
	public:
		virtual ~IComponent() = default;
		virtual inline ComponentType	GetType() = 0;
		IActor* GetOwner() {			return m_pOwner;		};
		void SetOwner(IActor* pActor) {			m_pOwner = pActor;		};
	private:
		IActor* m_pOwner;
	};
	template<class T>
	class Component: public IComponent
	{
	public:
		static const ComponentType Type;
		virtual inline ComponentType	GetType()
		{
			return Type;
		}
	};

	template<class T> ComponentType Component<T>::Type = typeid(T).hash_code();
	/// really simple component

	class ITransformComponent :public Component<ITransformComponent>
	{
	public:
		glm::mat4 transform;
	};


	class IMeshRenderComponent : public Component<IMeshRenderComponent>
	{
	public:
		render::ModelRender* m_pModel;
	};
}
