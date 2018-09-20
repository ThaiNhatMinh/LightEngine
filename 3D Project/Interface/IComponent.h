#pragma once

#include <glm\mat4x4.hpp>
#include "..\Utilities\Utility.h"
#include "..\typedef.h"
#include "..\Graphics3D\ICamera.h"
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
	protected:
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
		// +Z axis
		virtual glm::vec3 GetFront() = 0;
		// +X axis
		virtual glm::vec3 GetRight() = 0;
		// +Y axis
		virtual glm::vec3 GetUp() = 0;
		// Current position
		virtual glm::vec3 GetPos() = 0;
	};

	namespace render
	{
		class Model;
	}

	class IMeshRenderComponent : public Component<IMeshRenderComponent>
	{
	public:
		render::Model* m_pModel;
	};

	class ICameraComponent : public Component<ICameraComponent>, public render::ICamera
	{
	public:
		glm::mat4* m_GlobalTransform;
	};


	// respone for transition between animation clip, store animation state machine 
	class IAnimatorComponent : public Component<IAnimatorComponent>
	{
	public:
		virtual void Play(const std::string& name, bool loop, const std::string& layer = "Default") = 0;
	};
}
