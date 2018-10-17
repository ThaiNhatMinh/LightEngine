#pragma once

#include <glm\mat4x4.hpp>
#include "..\Utilities\Utility.h"
#include "..\typedef.h"
#include "ICamera.h"
#include "..\Math\AABB.h"

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
		
		
		virtual glm::vec3	GetFront() = 0;		// +Z axis
		virtual glm::vec3	GetRight() = 0;		// +X axis
		virtual glm::vec3	GetUp() = 0;		// +Y axis
		virtual glm::vec3	GetPos() = 0;		// Current position
		virtual glm::vec3	GetScale() = 0;
		
		virtual glm::mat4	GetTransform() = 0;
		virtual glm::quat	GetOrientation() = 0;
		virtual void		SetScale(glm::vec3 s) = 0;
		virtual void		SetPos(glm::vec3 pos) = 0;
		virtual void		SetOrientation(glm::quat ort) = 0;
		virtual void		SetTransform(glm::vec3 pos, glm::quat quad) = 0;
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

		virtual math::AABB GetBox() = 0;
	};

	class IScriptComponent : public Component<IScriptComponent>
	{
	public:
		virtual const std::string& VGetFile() = 0;
	};

	enum ShapeType
	{
		SHAPE_NONE,
		SHAPE_BOX,
		SHAPE_SPHERE,
		SHAPE_CAPSULE,
		SHAPE_CYLINDER,
		SHAPE_STATICPLANE,
		SHAPE_CONE,
		SHAPE_TRIANGLEMESH,
		SHAPE_CONVEXHULL,
		SHAPE_TERRAIN,
		SHAPE_CHARACTER
	};

	class IColliderComponent : public Component<IColliderComponent>
	{
	public:
		virtual ShapeType GetShapeType() = 0;
	};


	class IRigidBodyComponent : public Component<IRigidBodyComponent>
	{
	public:
		
	};

	class ISoundListener : public Component<ISoundListener>
	{

	};

	class ISoundSource3D : public Component<ISoundSource3D>
	{
	public:
		virtual bool Play(const std::string& name) = 0;
		virtual bool Play(const std::string& name, const glm::vec3& pos) = 0;
		virtual bool AddSound(const std::string& name) = 0;
	};
}
