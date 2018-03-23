#pragma once
#include <pch.h>

enum CollisionType
{
	TYPE_TEAMEMATE = 1 << 1,
	TYPE_ENEMY = 1 << 2,
	TYPE_WORLD = 1 << 3,
	TYPE_HITBOX = 1 << 4,

};

struct ActorMotionState : public btMotionState
{
	mat4 m_worldToPositionTransform;

	ActorMotionState(mat4 const & startingTransform)
		: m_worldToPositionTransform(startingTransform) { }

	// btMotionState interface:  Bullet calls these
	virtual void getWorldTransform(btTransform& worldTrans) const
	{
		worldTrans = Mat4x4_to_btTransform(m_worldToPositionTransform);
	}

	virtual void setWorldTransform(const btTransform& worldTrans)
	{
		m_worldToPositionTransform = btTransform_to_Mat4x4(worldTrans);
	}
};

const float WORLD_GRAVITY = 525;

struct MaterialData
{
	float m_restitution;
	float m_friction;

	MaterialData(float restitution, float friction)
	{
		m_restitution = restitution;
		m_friction = friction;
	}

	MaterialData(const MaterialData& other)
	{
		m_restitution = other.m_restitution;
		m_friction = other.m_friction;
	}
};

class HitBox;
/// Physics raycast hit.
struct PhysicsRaycastResult
{
	/// Construct with defaults.
	PhysicsRaycastResult() :
		body(0)
	{
	}

	/// Test for inequality, added to prevent GCC from complaining.
	bool operator !=(const PhysicsRaycastResult& rhs) const
	{
		return position != rhs.position || normal != rhs.normal || distance != rhs.distance || body != rhs.body;
	}

	/// Hit worldspace position.
	vec3 position;
	/// Hit worldspace normal.
	vec3 normal;
	/// Hit distance from ray origin.
	float distance;
	/// Hit fraction.
	float hitFraction;
	/// Rigid body that was hit.
	HitBox* body;
};


class BulletPhysics : public IGamePhysics, public ISubSystem
{
private:
	EventManager * m_pEventManager;

	friend class CharacterControllerComponent;
	friend class RigidBodyComponent;;
	// use auto pointers to automatically call delete on these objects
	//   during ~BulletPhysics

	// these are all of the objects that Bullet uses to do its work.
	//   see BulletPhysics::VInitialize() for some more info.
	std::unique_ptr<btDiscreteDynamicsWorld>         m_dynamicsWorld;
	std::unique_ptr<btBroadphaseInterface>           m_broadphase;
	std::unique_ptr<btCollisionDispatcher>           m_dispatcher;
	std::unique_ptr<btConstraintSolver>              m_solver;
	std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
	std::unique_ptr<BulletDebugDrawer>               m_debugDrawer;

	// tables read from the XML
	typedef std::map<std::string, float> DensityTable;
	typedef std::map<std::string, MaterialData> MaterialTable;
	DensityTable m_densityTable;
	MaterialTable m_materialTable;

	void LoadXml();
	float LookupSpecificGravity(const std::string& densityStr);
	MaterialData LookupMaterialData(const std::string& materialStr);

	// keep track of the existing rigid bodies:  To check them for updates
	//   to the actors' positions, and to remove them when their lives are over.
	typedef std::map<ActorId, RigidBodyComponent*> ActorIDToRigidBodyMap;
	ActorIDToRigidBodyMap m_actorIdToRigidBody;
	RigidBodyComponent * FindBulletRigidBody(ActorId id) const;

	// also keep a map to get the actor id from the btRigidBody*
	typedef std::map<RigidBodyComponent const *, ActorId> RigidBodyToActorIDMap;
	RigidBodyToActorIDMap m_rigidBodyToActorId;
	ActorId FindActorID(RigidBodyComponent const *) const;
	//ActorId FindActorID(btRigidBody const * const body) const;
	// data used to store which collision pair (bodies that are touching) need
	//   Collision events sent.  When a new pair of touching bodies are detected,
	//   they are added to m_previousTickCollisionPairs and an event is sent.
	//   When the pair is no longer detected, they are removed and another event
	//   is sent.
	typedef std::pair< btRigidBody const *, btRigidBody const * > CollisionPair;
	typedef std::set< CollisionPair > CollisionPairs;
	CollisionPairs m_previousTickCollisionPairs;

	// helpers for sending events relating to collision pairs
	void SendCollisionEvents();
	//void SendCollisionPairAddEvent(btPersistentManifold const * manifold, btRigidBody const * body0, btRigidBody const * body1);
	//void SendCollisionPairRemoveEvent(RigidBodyComponent const * body0, RigidBodyComponent const * body1);
	
	// common functionality used by VAddSphere, VAddBox, etc
	//void AddShape(Actor* pGameActor, btCollisionShape* shape, float mass, const std::string& physicsMaterial);
	void AddRigidBody(ActorId id,RigidBodyComponent* rb);
	
	// helper for cleaning up objects
	void RemoveCollisionObject(btCollisionObject * removeMe);

	
	// callback from bullet for each physics time step.  set in VInitialize
	static void BulletInternalTickCallback(btDynamicsWorld * const world, btScalar const timeStep);
	// callback from bullet for each physics time step.  set in VInitialize
	static void BulletInternalPreTickCallback(btDynamicsWorld * const world, btScalar const timeStep);

protected:
	
public:
	BulletPhysics(Context* c);				// [mrmike] This was changed post-press to add event registration!
	virtual ~BulletPhysics();

	

	// Initialiazation and Maintenance of the Physics World
	virtual bool VInitialize() override { return 1; };
	virtual void VSyncVisibleScene() override;
	virtual void VOnUpdate(float deltaSeconds) override;
	virtual void VPostStep(float timeStep) override;
	virtual void VPreStep(float timeStep) override;
	
	virtual void VRemoveActor(ActorId id) override;
	// Debugging
	virtual void VRenderDiagnostics() override;

	virtual void RayCast(PhysicsRaycastResult& result, const Ray& r, float maxdistance, unsigned mask = 0xffffffff);
	virtual void RayCast(std::vector<PhysicsRaycastResult>& result, const Ray& r, float maxdistance, unsigned mask = 0xffffffff);

	btCollisionWorld* GetCollisionWorld();
};

