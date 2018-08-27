#pragma once

#include <glm\mat4x4.hpp>
#include "..\Utilities\Utility.h"
#include "..\typedef.h"
#include "../Graphics3D/ModelRender.h"
namespace Light 
{
	class IActor;
	class IComponent: public util::Serialization
	{
	public:
		virtual ~IComponent() = default;
		virtual ComponentType	GetType() = 0;
		IActor* GetOwner() {			return m_pOwner;		};
		void SetOwner(IActor* pActor) {			m_pOwner = pActor;		};
	private:
		IActor* m_pOwner;
	};


	template<class T>
	class Component: public IComponent
	{
	public:
		static const ComponentType StaticType;
		virtual ComponentType	GetType()
		{
			return StaticType;
		}
	};

	template<class T> const ComponentType Component<T>::StaticType = typeid(T).hash_code();
	
	
	/// really simple component
	class ITransformComponent :public Component<ITransformComponent>
	{
	public:
		glm::mat4 transform;
	};


	class IMeshRenderComponent : public Component<IMeshRenderComponent>
	{
	public:
		render::Model* m_pModel;
	};
}
