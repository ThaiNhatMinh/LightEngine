#pragma once
#include <pch.h>

// Bullet Physic 
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "..\Interface\IGamePhysic.h"
#include "..\Interface\IEventManager.h"
#include "..\Interface\IDebugRender.h"
#include "..\Math\Ray.h"
#include "..\Utilities\PhysicsUtilities.h"
#include "..\Interface\IContext.h"

namespace Light
{
namespace physics
{


	struct ActorMotionState : public btMotionState
	{
		mat4 m_worldToPositionTransform;
		glm::vec3 position;
		glm::quat orientation;
		ActorMotionState(mat4 const & startingTransform)
			: m_worldToPositionTransform(startingTransform) 
		{
			position = startingTransform[3];
			orientation = glm::toQuat(startingTransform);
		}

		// btMotionState interface:  Bullet calls these
		virtual void getWorldTransform(btTransform& worldTrans) const
		{
			//worldTrans = Mat4x4_to_btTransform(m_worldToPositionTransform);
			worldTrans.setOrigin(ToBtVector3(position));
			worldTrans.setRotation(ToBtQuaternion(orientation));
			///worldTrans = btTransform(ToBtQuaternion(orientation), ToBtVector3(position));
		}

		virtual void setWorldTransform(const btTransform& worldTrans)
		{
			position = ToVector3(worldTrans.getOrigin());
			orientation = ToQuaternion(worldTrans.getRotation());

			//m_worldToPositionTransform = btTransform_to_Mat4x4(worldTrans);
		}
	};


	class BulletPhysics : public IGamePhysic, public btIDebugDraw
	{
	private:
		IEventManager * m_pEventManager;

		friend class CharacterControllerComponent;
		friend class RigidBodyComponent;
		std::unique_ptr<btDiscreteDynamicsWorld>         m_dynamicsWorld;
		std::unique_ptr<btBroadphaseInterface>           m_broadphase;
		std::unique_ptr<btCollisionDispatcher>           m_dispatcher;
		std::unique_ptr<btConstraintSolver>              m_solver;
		std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;

		// tables read from the XML
		typedef std::map<std::string, float> DensityTable;
		typedef std::map<std::string, MaterialData> MaterialTable;
		DensityTable m_densityTable;
		MaterialTable m_materialTable;

		void LoadXml();
		float LookupSpecificGravity(const std::string& densityStr);
		MaterialData LookupMaterialData(const std::string& materialStr);

		typedef std::map<ActorId, RigidBodyComponent*> ActorIDToRigidBodyMap;
		ActorIDToRigidBodyMap m_actorIdToRigidBody;
		RigidBodyComponent * FindBulletRigidBody(ActorId id) const;

		// also keep a map to get the actor id from the btRigidBody*
		typedef std::map<RigidBodyComponent const *, ActorId> RigidBodyToActorIDMap;
		RigidBodyToActorIDMap m_rigidBodyToActorId;
		ActorId FindActorID(RigidBodyComponent const *) const;
		
		typedef std::pair< btRigidBody const *, btRigidBody const * > CollisionPair;
		typedef std::set< CollisionPair > CollisionPairs;
		CollisionPairs m_previousTickCollisionPairs;

		// helpers for sending events relating to collision pairs
		void SendCollisionEvents();
		//void SendCollisionPairAddEvent(btPersistentManifold const * manifold, btRigidBody const * body0, btRigidBody const * body1);
		//void SendCollisionPairRemoveEvent(RigidBodyComponent const * body0, RigidBodyComponent const * body1);

		// common functionality used by VAddSphere, VAddBox, etc
		//void AddShape(Actor* pGameActor, btCollisionShape* shape, float mass, const std::string& physicsMaterial);
		void AddRigidBody(ActorId id, IRigidBodyComponent* rb);

		// helper for cleaning up objects
		void RemoveCollisionObject(btCollisionObject * removeMe);


		// callback from bullet for each physics time step.  set in VInitialize
		static void BulletInternalTickCallback(btDynamicsWorld * const world, btScalar const timeStep);
		// callback from bullet for each physics time step.  set in VInitialize
		static void BulletInternalPreTickCallback(btDynamicsWorld * const world, btScalar const timeStep);

	protected:

		DebugDrawModes              m_DebugModes;
		virtual void	drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
		virtual void	reportErrorWarning(const char* warningString) override;
		virtual void	draw3dText(const btVector3& location, const char* textString) override;
		virtual void	setDebugMode(int debugMode) override;
		virtual int		getDebugMode() const override;
		virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

		IDebugRender* m_pDebuger;
	public:
		BulletPhysics(IContext * c);				// [mrmike] This was changed post-press to add event registration!
		virtual ~BulletPhysics();



		// Initialiazation and Maintenance of the Physics World
		virtual void VSyncVisibleScene() override;
		virtual void VOnUpdate(float deltaSeconds) override;
		virtual void VPostStep(float timeStep);
		virtual void VPreStep(float timeStep);

		virtual void VRemoveActor(ActorId id) override;
		// Debugging
		virtual void VRenderDiagnostics() override;

		virtual void RayCast(PhysicsRaycastResult& result, const  math::Ray& r, float maxdistance, unsigned mask = 0xffffffff);
		virtual void RayCast(std::vector<PhysicsRaycastResult>& result, const  math::Ray& r, float maxdistance, unsigned mask = 0xffffffff);

		virtual glm::vec3 VGetGravity();
		virtual const char* VGetName();
		btCollisionWorld* GetCollisionWorld();
	};

}
}