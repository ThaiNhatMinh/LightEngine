#pragma once
#include "..\Interface\IComponent.h"
#include "..\Interface\IGamePhysic.h"
// Bullet Physic 
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Light
{
	namespace physics
	{
		class RigidBodyComponent : public IRigidBodyComponent
		{
			friend class BulletPhysics;
		public:
			RigidBodyComponent(void);
			virtual ~RigidBodyComponent(void);

			virtual bool VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
			virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);

			//// ActorComponent interface
			//virtual bool VInit(const tinyxml2::XMLElement* pData) override;
			void PostInit(void);
			//virtual void VUpdate(float deltaMs) override;

			// Physic Interface
			btMotionState* GetMotionState();
			btRigidBody*	GetRigidBody();
			const btTransform& GetTransform();

			vec3 GetLinearVelocity();
			void SetLinearVelocity(const vec3& velocity);
			// Set linear degrees of freedom. Use 1 to enable an axis or 0 to disable. Default is all axes enabled (1, 1, 1).
			void SetLinearFactor(const vec3& factor);
			// Set linear velocity damping factor.
			void SetLinearDamping(float damping);
			// Set angular velocity.
			void SetAngularVelocity(const vec3& angularVelocity);
			// Set angular degrees of freedom. Use 1 to enable an axis or 0 to disable. Default is all axes enabled (1, 1, 1).
			void SetAngularFactor(const vec3& factor);
			// Set angular velocity damping factor.
			void SetAngularDamping(float factor);
			// Set friction coefficient.
			void SetFriction(float friction);
			// Set anisotropic friction.
			void SetAnisotropicFriction(const vec3& friction);
			// Set restitution coefficient.
			void SetRestitution(float restitution);
			// Set contact processing threshold.
			void SetContactProcessingThreshold(float threshold);
			// Set continuous collision detection swept sphere radius.
			void SetCcdRadius(float radius);
			// Set continuous collision detection motion-per-simulation-step threshold. 0 disables, which is the default.
			void SetCcdMotionThreshold(float threshold);
			// Set whether gravity is applied to rigid body.
			void SetUseGravity(bool enable);
			// Set gravity override. If zero, uses physics world's gravity.
			void SetCustomGravity(const vec3& gravity);
			// Set rigid body trigger mode. In trigger mode collisions are reported but do not apply forces.
			void SetTrigger(bool enable);
			// Apply force to center of mass.
			void ApplyForce(const vec3& force);
			// Apply force at local position.
			void ApplyForce(const vec3& force, const vec3& position);
			// Apply torque.
			void ApplyTorque(const vec3& torque);
			// Apply impulse to center of mass.
			void ApplyImpulse(const vec3& impulse);
			// Apply impulse at local position.
			void ApplyImpulse(const vec3& impulse, const vec3& position);
			// Apply torque impulse.
			void ApplyTorqueImpulse(const vec3& torque);
			// Reset accumulated forces.
			void ResetForces();
			// Activate rigid body if it was resting.
			void Activate();
		protected:
			void UpdateGravity();
		private:

			void BuildRigidBodyTransform(const tinyxml2::XMLElement* pTransformElement);

			IGamePhysic* m_pPhysic;
			btRigidBody*	m_pRigidBody;
			vec3			m_CenterOfMass;
			//std::string		m_shape;
			//std::string		m_density;
			std::string		m_material;

			vec3			m_RigidBodyLocation;		// this isn't world position! This is how the rigid body is offset from the position of the actor.
			glm::quat		m_RigidBodyOrientation;		// ditto, orientation
			float			m_fMass;
			vec3			m_CustomGravity;			// Gravity for this RigidBody
			int				m_Flags;					// Bullet physic flag
			bool			m_bUseGravity;

		};
	}
}

