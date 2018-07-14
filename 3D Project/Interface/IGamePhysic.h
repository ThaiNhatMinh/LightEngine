#pragma once


namespace Light
{
	namespace physics
	{

		const float WORLD_GRAVITY = 525;

		enum CollisionType
		{
			TYPE_TEAMEMATE = 1 << 1,
			TYPE_ENEMY = 1 << 2,
			TYPE_WORLD = 1 << 3,
			TYPE_HITBOX = 1 << 4,

		};

		//struct ActorMotionState : public btMotionState
		//{
		//	mat4 m_worldToPositionTransform;

		//	ActorMotionState(mat4 const & startingTransform)
		//		: m_worldToPositionTransform(startingTransform) { }

		//	// btMotionState interface:  Bullet calls these
		//	virtual void getWorldTransform(btTransform& worldTrans) const
		//	{
		//		worldTrans = Mat4x4_to_btTransform(m_worldToPositionTransform);
		//	}

		//	virtual void setWorldTransform(const btTransform& worldTrans)
		//	{
		//		m_worldToPositionTransform = btTransform_to_Mat4x4(worldTrans);
		//	}
		//};

		//

		//struct MaterialData
		//{
		//	float m_restitution;
		//	float m_friction;

		//	MaterialData(float restitution, float friction)
		//	{
		//		m_restitution = restitution;
		//		m_friction = friction;
		//	}

		//	MaterialData(const MaterialData& other)
		//	{
		//		m_restitution = other.m_restitution;
		//		m_friction = other.m_friction;
		//	}
		//};

		//class HitBox;
		///// Physics raycast hit.
		//struct PhysicsRaycastResult
		//{
		//	/// Construct with defaults.
		//	PhysicsRaycastResult() :
		//		body(0)
		//	{
		//	}

		//	/// Test for inequality, added to prevent GCC from complaining.
		//	bool operator !=(const PhysicsRaycastResult& rhs) const
		//	{
		//		return position != rhs.position || normal != rhs.normal || distance != rhs.distance || body != rhs.body;
		//	}

		//	/// Hit worldspace position.
		//	vec3 position;
		//	/// Hit worldspace normal.
		//	vec3 normal;
		//	/// Hit distance from ray origin.
		//	float distance;
		//	/// Hit fraction.
		//	float hitFraction;
		//	/// Rigid body that was hit.
		//	HitBox* body;
		//};

		class IGamePhysic: public ISubSystem
		{
		public:
			virtual void VSyncVisibleScene() = 0;
			virtual void VOnUpdate(float deltaSeconds) = 0;
			virtual void VPreStep(float timeStep) = 0;
			virtual void VPostStep(float timeStep) = 0;

			virtual void VRemoveActor(ActorId id) = 0;
			// Debugging
			virtual void VRenderDiagnostics() = 0;
		};
	}
}