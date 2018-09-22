#include <pch.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision\CollisionDispatch\btInternalEdgeUtility.h>
#include "Physic.h"
#include "Events.h"
#include "..\Utilities\PhysicsUtilities.h"

#include "..\GameComponents\RigidBodyComponent.h"
namespace Light
{
namespace physics
{
// helpers for conversion to and from Bullet's data types
static btVector3 Vec3_to_btVector3(vec3 const & v)
{
	return btVector3(v.x, v.y, v.z);
}

static vec3 btVector3_to_Vec3(btVector3 const & btvec)
{
	return vec3(btvec.x(), btvec.y(), btvec.z());
}

ContactAddedCallback      gContactAddedCallback;

static bool CustomMaterialCombinerCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0,
	int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
{
	btAdjustInternalEdgeContacts(cp, colObj1Wrap, colObj0Wrap, partId1, index1);

	cp.m_combinedFriction = colObj0Wrap->getCollisionObject()->getFriction() * colObj1Wrap->getCollisionObject()->getFriction();
	cp.m_combinedRestitution =
		colObj0Wrap->getCollisionObject()->getRestitution() * colObj1Wrap->getCollisionObject()->getRestitution();

	return true;
}

BulletPhysics::BulletPhysics(IContext * c)
{
	/*REGISTER_EVENT(EvtData_PhysTrigger_Enter);
	REGISTER_EVENT(EvtData_PhysTrigger_Leave);
	REGISTER_EVENT(EvtData_PhysCollisionStart);
	REGISTER_EVENT(EvtData_PhysOnCollision);
	REGISTER_EVENT(EvtData_PhysCollisionEnd);*/

	m_pEventManager = c->GetSystem<IEventManager>();
	m_pDebuger = c->GetSystem<IDebugRender>();
	LoadXml();

	gContactAddedCallback = CustomMaterialCombinerCallback;

	// this controls how Bullet does internal memory management during the collision pass
	m_collisionConfiguration = std::unique_ptr<btDefaultCollisionConfiguration>(DEBUG_NEW btDefaultCollisionConfiguration());

	// this manages how Bullet detects precise collisions between pairs of objects
	m_dispatcher = std::unique_ptr<btCollisionDispatcher>(DEBUG_NEW btCollisionDispatcher(m_collisionConfiguration.get()));

	// Bullet uses this to quickly (imprecisely) detect collisions between objects.
	//   Once a possible collision passes the broad phase, it will be passed to the
	//   slower but more precise narrow-phase collision detection (btCollisionDispatcher).
	m_broadphase = std::unique_ptr<btDbvtBroadphase>(DEBUG_NEW btDbvtBroadphase());

	// Manages constraints which apply forces to the physics simulation.  Used
	//  for e.g. springs, motors.  We don't use any constraints right now.
	m_solver = std::unique_ptr<btSequentialImpulseConstraintSolver>(new btSequentialImpulseConstraintSolver);

	// This is the main Bullet interface point.  Pass in all these components to customize its behavior.
	m_dynamicsWorld = std::unique_ptr<btDiscreteDynamicsWorld>(new btDiscreteDynamicsWorld(m_dispatcher.get(),
		m_broadphase.get(),
		m_solver.get(),
		m_collisionConfiguration.get()));

	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

	
	m_DebugModes = btIDebugDraw::DBG_DrawWireframe;
	//m_debugDrawer->ReadOptions();

	if (!m_collisionConfiguration || !m_dispatcher || !m_broadphase ||
		!m_solver || !m_dynamicsWorld)
	{
		E_ERROR("BulletPhysics::VInitialize failed!");
		return;
	}

	m_dynamicsWorld->setDebugDrawer(this);


	// and set the internal tick callback to our own method "BulletInternalTickCallback"
	m_dynamicsWorld->setInternalTickCallback(BulletInternalTickCallback, static_cast<void*>(this), false);
	m_dynamicsWorld->setInternalTickCallback(BulletInternalPreTickCallback, static_cast<void*>(this), true);
	m_dynamicsWorld->setWorldUserInfo(this);
	m_dynamicsWorld->getDispatchInfo().m_useContinuous = true;
	m_dynamicsWorld->getSolverInfo().m_splitImpulse = false; // Disable by default for performance
	m_dynamicsWorld->setSynchronizeAllMotionStates(true);
	//m_dynamicsWorld->debugDrawWorld();

	c->VAddSystem(this);
}

/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::~BulletPhysics				
//
BulletPhysics::~BulletPhysics()
{
	//E_DEBUG("Physic Engine Shutdown...");
	// delete any physics objects which are still in the world

	// iterate backwards because removing the last object doesn't affect the
	//  other objects stored in a vector-type array
	for (int ii = m_dynamicsWorld->getNumCollisionObjects() - 1; ii >= 0; --ii)
	{
		btCollisionObject * const obj = m_dynamicsWorld->getCollisionObjectArray()[ii];

		RemoveCollisionObject(obj);
	}

	m_rigidBodyToActorId.clear();
}

/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::LoadXml						
//
//    Loads the physics materials from an XML file
//
void BulletPhysics::LoadXml()
{
	// Load the physics config file and grab the root XML node
	tinyxml2::XMLDocument doc;
	doc.LoadFile("Configs\\physics.xml");

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("Physics");

	// load all materials
	tinyxml2::XMLElement* pParentNode = pRoot->FirstChildElement("PhysicsMaterials");
	//GCC_ASSERT(pParentNode);
	for (tinyxml2::XMLElement* pNode = pParentNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		double restitution = pNode->DoubleAttribute("restitution");
		double friction = pNode->DoubleAttribute("friction");
		
		
		m_materialTable.insert(std::make_pair(pNode->Value(), MaterialData((float)restitution, (float)friction)));
	}

	// load all densities
	pParentNode = pRoot->FirstChildElement("DensityTable");
	//GCC_ASSERT(pParentNode);
	for (tinyxml2::XMLElement* pNode = pParentNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		m_densityTable.insert(std::make_pair(pNode->Value(), (float)atof(pNode->FirstChild()->Value())));
	}
	
}



/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::VOnUpdate						
//
void BulletPhysics::VOnUpdate(float const deltaSeconds)
{
	// Bullet uses an internal fixed timestep (default 1/60th of a second)
	//   We pass in 4 as a max number of sub steps.  Bullet will run the simulation
	//   in increments of the fixed timestep until "deltaSeconds" amount of time has
	//   passed, but will only run a maximum of 4 steps this way.
	m_dynamicsWorld->stepSimulation(deltaSeconds,4);
	//E_DEBUG("Physuic Update()");
	

}


void BulletPhysics::VRenderDiagnostics()
{
	m_dynamicsWorld->debugDrawWorld();
}

void BulletPhysics::RayCast(PhysicsRaycastResult & result, const math::Ray & r, float maxdistance, unsigned mask)
{
	btCollisionWorld::ClosestRayResultCallback raycallback(ToBtVector3(r.pos), ToBtVector3(r.pos + r.direction*maxdistance));
	raycallback.m_collisionFilterGroup = 0xffff;
	raycallback.m_collisionFilterMask = (short)mask;

	m_dynamicsWorld->rayTest(raycallback.m_rayFromWorld, raycallback.m_rayToWorld, raycallback);
	
	if (raycallback.hasHit())
	{
		result.position = ToVector3(raycallback.m_hitPointWorld);
		result.normal = ToVector3(raycallback.m_hitNormalWorld);
		result.distance = (result.position - r.pos).length();
		result.hitFraction = raycallback.m_closestHitFraction;
		result.body = static_cast<HitBox*>(raycallback.m_collisionObject->getUserPointer());
		//cout << "Hit name: " << raycallback.m_collisionObject->getCollisionShape()->getName() << endl;
		
	}
	else
	{
		result.position = vec3(0);
		result.normal = vec3(0);
		result.distance = INFINITY;
		result.hitFraction = 0.0f;
		result.body = 0;
	}
}

void BulletPhysics::RayCast(std::vector<PhysicsRaycastResult>& result, const  math::Ray & r, float maxdistance, unsigned mask)
{
	btCollisionWorld::AllHitsRayResultCallback
		rayCallback(ToBtVector3(r.pos), ToBtVector3(r.pos + maxdistance * r.direction));
	rayCallback.m_collisionFilterGroup = (short)0xffff;
	rayCallback.m_collisionFilterMask = (short)mask;

	m_dynamicsWorld->rayTest(rayCallback.m_rayFromWorld, rayCallback.m_rayToWorld, rayCallback);

	
	for (int i = 0; i < rayCallback.m_collisionObjects.size(); ++i)
	{
		PhysicsRaycastResult newResult;
		newResult.body = static_cast<HitBox*>(rayCallback.m_collisionObjects[i]->getUserPointer());
		newResult.position = ToVector3(rayCallback.m_hitPointWorld[i]);
		newResult.normal = ToVector3(rayCallback.m_hitNormalWorld[i]);
		newResult.distance = (newResult.position - r.pos).length();
		newResult.hitFraction = rayCallback.m_closestHitFraction;
		result.push_back(newResult);
	}

	auto CompareRaycastResults = [](const PhysicsRaycastResult& lhs, const PhysicsRaycastResult& rhs)
	{
		return lhs.distance < rhs.distance;
	};
	std::sort(result.begin(), result.end(), CompareRaycastResults);
}

glm::vec3 Light::physics::BulletPhysics::VGetGravity()
{
	return ToVector3(m_dynamicsWorld->getGravity());
}

const char * Light::physics::BulletPhysics::VGetName()
{
	static const char * t = typeid(IGamePhysic).name();
	return t;
}

btCollisionWorld * BulletPhysics::GetCollisionWorld()
{
	return m_dynamicsWorld.get();
}

void BulletPhysics::VPostStep(float timeStep)
{


	SendCollisionEvents();


	std::shared_ptr<IEvent> pEvent(DEBUG_NEW events::EvtPhysPostStep(timeStep));
	m_pEventManager->VTriggerEvent(pEvent);
}

void BulletPhysics::VPreStep(float timeStep)
{
	std::shared_ptr<IEvent>pEvent(DEBUG_NEW events::EvtPhysPreStep(timeStep));
	m_pEventManager->VTriggerEvent(pEvent);
}

/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::VSyncVisibleScene				
//
void BulletPhysics::VSyncVisibleScene()
{
	// Keep physics & graphics in sync

	// check all the existing actor's bodies for changes. 
	//  If there is a change, send the appropriate event for the game system.
	for (ActorIDToRigidBodyMap::const_iterator it = m_actorIdToRigidBody.begin(); it != m_actorIdToRigidBody.end();	++it)
	{
		ActorId const id = it->first;

		ActorMotionState const * const actorMotionState = static_cast<ActorMotionState*>(it->second->GetMotionState());
		//GCC_ASSERT(actorMotionState);
		IActor* pGameActor = 0;
		RigidBodyComponent *pRb = FindBulletRigidBody(id);
		pGameActor = pRb->GetOwner();
		ITransformComponent* pTransformComponent = pGameActor->GetComponent<ITransformComponent>();
		if (pTransformComponent)
		{
			if (pTransformComponent->GetPos() != actorMotionState->position || pTransformComponent->GetOrientation() != actorMotionState->orientation)
			{
				// Bullet has moved the actor's physics object.  Sync the transform and inform the game an actor has moved
				pTransformComponent->SetPos(actorMotionState->position);
				pTransformComponent->SetOrientation(actorMotionState->orientation);

				std::shared_ptr<IEvent> pEvent(DEBUG_NEW events::EvtMoveActor(pGameActor, actorMotionState->m_worldToPositionTransform));
				m_pEventManager->VQueueEvent(pEvent);
			}
		}
	}
}

void BulletPhysics::AddRigidBody(ActorId id, IRigidBodyComponent * rb)
{
	auto r = static_cast<RigidBodyComponent*>(rb);
	m_dynamicsWorld->addRigidBody(r->m_pRigidBody);
	m_actorIdToRigidBody[id] = r;
	m_rigidBodyToActorId[r] = id;
}
/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::RemoveCollisionObject			
//
//    Removes a collision object from the game world
//
void BulletPhysics::RemoveCollisionObject(btCollisionObject * const removeMe)
{
	// first remove the object from the physics sim
	m_dynamicsWorld->removeCollisionObject(removeMe);

	// then remove the pointer from the ongoing contacts list
	for (CollisionPairs::iterator it = m_previousTickCollisionPairs.begin();
	it != m_previousTickCollisionPairs.end(); )
	{
		CollisionPairs::iterator next = it;
		++next;

		if (it->first == removeMe || it->second == removeMe)
		{
			//SendCollisionPairRemoveEvent(it->first, it->second);
			m_previousTickCollisionPairs.erase(it);
		}

		it = next;
	}

	// if the object is a RigidBody (all of ours are RigidBodies, but it's good to be safe)
	if (btRigidBody * const body = btRigidBody::upcast(removeMe))
	{
		// delete the components of the object
		delete body->getMotionState();
		delete body->getCollisionShape();

		for (int ii = body->getNumConstraintRefs() - 1; ii >= 0; --ii)
		{
			btTypedConstraint * const constraint = body->getConstraintRef(ii);
			m_dynamicsWorld->removeConstraint(constraint);
			delete constraint;
		}
	}

	delete removeMe;
}

/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::FindBulletRigidBody			
//    Finds a Bullet rigid body given an actor ID
//
RigidBodyComponent* BulletPhysics::FindBulletRigidBody(ActorId const id) const
{
	ActorIDToRigidBodyMap::const_iterator found = m_actorIdToRigidBody.find(id);
	if (found != m_actorIdToRigidBody.end())
		return found->second;

	return NULL;
}

ActorId BulletPhysics::FindActorID(RigidBodyComponent const * const body) const
{
	RigidBodyToActorIDMap::const_iterator found = m_rigidBodyToActorId.find(body);
	if (found != m_rigidBodyToActorId.end())
		return found->second;

	return ActorId();
}


/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::VRemoveActor					
//
//    Implements the method to remove actors from the physics simulation
//
void BulletPhysics::VRemoveActor(ActorId id)
{
	if (RigidBodyComponent * const body = FindBulletRigidBody(id))
	{
		// destroy the body and all its components
		RemoveCollisionObject(body->GetRigidBody());
		m_actorIdToRigidBody.erase(id);
		m_rigidBodyToActorId.erase(body);
	}
}


/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::BulletInternalTickCallback
//
// This function is called after bullet performs its internal update.  We
//   use it to detect collisions between objects for Game code.
//
void BulletPhysics::BulletInternalTickCallback(btDynamicsWorld * const world, btScalar const timeStep)
{
	//E_DEBUG("BulletInternalTickCallback()");
	assert(world);

	assert(world->getWorldUserInfo());
	BulletPhysics * const bulletPhysics = static_cast<BulletPhysics*>(world->getWorldUserInfo());

	bulletPhysics->VPostStep(timeStep);

}

void BulletPhysics::BulletInternalPreTickCallback(btDynamicsWorld * const world, btScalar const timeStep)
{
	//E_DEBUG("BulletInternalPreTickCallback()");
	assert(world);

	assert(world->getWorldUserInfo());
	BulletPhysics * const bulletPhysics = static_cast<BulletPhysics*>(world->getWorldUserInfo());
	bulletPhysics->VPreStep(timeStep);
}

void BulletPhysics::SendCollisionEvents()
{
	CollisionPairs currentTickCollisionPairs;

	// look at all existing contacts
	btDispatcher * const dispatcher = m_dynamicsWorld->getDispatcher();
	for (int manifoldIdx = 0; manifoldIdx < dispatcher->getNumManifolds(); ++manifoldIdx)
	{
		// get the "manifold", which is the set of data corresponding to a contact point
		//   between two physics objects
		btPersistentManifold const * const manifold = dispatcher->getManifoldByIndexInternal(manifoldIdx);
		assert(manifold);

		// get the two bodies used in the manifold.  Bullet stores them as void*, so we must cast
		//  them back to btRigidBody*s.  Manipulating void* pointers is usually a bad
		//  idea, but we have to work with the environment that we're given.  We know this
		//  is safe because we only ever add btRigidBodys to the simulation
		btRigidBody const * const body0 = static_cast<btRigidBody const *>(manifold->getBody0());
		btRigidBody const * const body1 = static_cast<btRigidBody const *>(manifold->getBody1());

		// always create the pair in a predictable order
		bool const swapped = body0 > body1;

		btRigidBody const * const sortedBodyA = swapped ? body1 : body0;
		btRigidBody const * const sortedBodyB = swapped ? body0 : body1;

		CollisionPair const thisPair = std::make_pair(sortedBodyA, sortedBodyB);
		currentTickCollisionPairs.insert(thisPair);

		RigidBodyComponent* bodyC0 = static_cast<RigidBodyComponent*>(body0->getUserPointer());
		RigidBodyComponent* bodyC1 = static_cast<RigidBodyComponent*>(body1->getUserPointer());
		// this is a new contact, which wasn't in our list before.  send an event to the game.
		ActorId const id0 = bodyC0->GetOwner()->VGetId();
		ActorId const id1 = bodyC1->GetOwner()->VGetId();

		if (id0 == 0 || id1 == 0)
		{
			E_WARNING("something is colliding with a non-actor.  we currently don't send events for that");
			return;
		}

		// this pair of colliding objects is new.  send a collision-begun event
		vector<PhysicCollisionData> collisionPoints;
		vec3 sumNormalForce;
		vec3 sumFrictionForce;

		for (int pointIdx = 0; pointIdx < manifold->getNumContacts(); ++pointIdx)
		{
			btManifoldPoint const & point = manifold->getContactPoint(pointIdx);
			PhysicCollisionData data( btVector3_to_Vec3(point.m_positionWorldOnB),btVector3_to_Vec3(point.m_normalWorldOnB) );
			collisionPoints.push_back(data);

			sumNormalForce += btVector3_to_Vec3(point.m_combinedRestitution * point.m_normalWorldOnB);
			sumFrictionForce += btVector3_to_Vec3(point.m_combinedFriction * point.m_lateralFrictionDir1);
		}

		if (m_previousTickCollisionPairs.find(thisPair) == m_previousTickCollisionPairs.end())
		{
			// this is new collision
			// send the event for the game
			std::shared_ptr<IEvent>  pEvent(DEBUG_NEW events::EvtPhysCollisionStart(bodyC0->GetOwner(), bodyC1->GetOwner(), sumNormalForce, sumFrictionForce, collisionPoints));
			m_pEventManager->VQueueEvent(pEvent);
		}
		
		// send the event for the game
		std::shared_ptr<IEvent>  pEvent( DEBUG_NEW events::EvtPhysOnCollision(bodyC0->GetOwner(), bodyC1->GetOwner(), sumNormalForce, sumFrictionForce, collisionPoints));
		m_pEventManager->VQueueEvent(pEvent);
		
	}

	CollisionPairs removedCollisionPairs;

	// use the STL set difference function to find collision pairs that existed during the previous tick but not any more
	std::set_difference(m_previousTickCollisionPairs.begin(), m_previousTickCollisionPairs.end(),
		currentTickCollisionPairs.begin(), currentTickCollisionPairs.end(),
		std::inserter(removedCollisionPairs, removedCollisionPairs.begin()));

	for (CollisionPairs::const_iterator it = removedCollisionPairs.begin(),
		end = removedCollisionPairs.end(); it != end; ++it)
	{
		btRigidBody const * const body0 = it->first;
		btRigidBody const * const body1 = it->second;

		RigidBodyComponent* bodyA = static_cast<RigidBodyComponent*>(body0->getUserPointer());
		RigidBodyComponent* bodyB = static_cast<RigidBodyComponent*>(body1->getUserPointer());

		ActorId const id0 = bodyA->GetOwner()->VGetId();
		ActorId const id1 = bodyB->GetOwner()->VGetId();

		if (id0 == 0 || id1 == 0)
		{
			// collision is ending between some object(s) that don't have actors.  we don't send events for that.
			return;
		}

		std::shared_ptr<IEvent>  pEvent(DEBUG_NEW events::EvtPhysCollisionEnd(id0, id1));
		m_pEventManager->VQueueEvent(pEvent);
	}

	// the current tick becomes the previous tick.  this is the way of all things.
	m_previousTickCollisionPairs = currentTickCollisionPairs;
}


float BulletPhysics::LookupSpecificGravity(const std::string& densityStr)
{
	float density = 0;
	auto densityIt = m_densityTable.find(densityStr);
	if (densityIt != m_densityTable.end())
		density = densityIt->second;
	// else: dump error

	return density;
}

MaterialData BulletPhysics::LookupMaterialData(const std::string& materialStr)
{
	auto materialIt = m_materialTable.find(materialStr);
	if (materialIt != m_materialTable.end())
		return materialIt->second;
	else
		return MaterialData(0, 0);
}


void BulletPhysics::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	// draw a line to represent the normal.  This only lasts one frame, and is hard to see.
	//   it might help to linger this drawn object onscreen for a while to make it more noticeable

	btVector3 const startPoint = PointOnB;
	btVector3 const endPoint = PointOnB + normalOnB * distance;

	drawLine(startPoint, endPoint, color);
}

void BulletPhysics::reportErrorWarning(const char* warningString)
{
	E_WARNING(warningString);
}

void BulletPhysics::draw3dText(const btVector3& location, const char* textString)
{
	// FUTURE WORK - BulletPhysics::draw3dText needs an implementation
}

void BulletPhysics::setDebugMode(int debugMode)
{
	m_DebugModes = (DebugDrawModes)debugMode;
}

int BulletPhysics::getDebugMode() const
{
	return m_DebugModes;
}
//
//void BulletPhysics::ReadOptions()
//{
//	
//	TiXmlDocument *optionsDoc = g_pApp->m_Options.m_pDoc;
//	TiXmlElement *pRoot = optionsDoc->RootElement();
//	if (!pRoot)
//		return;
//
//	int debugModes = btIDebugDraw::DBG_NoDebug;
//	TiXmlElement *pNode = pRoot->FirstChildElement("PhysicsDebug");
//	if (pNode)
//	{
//		if (pNode->Attribute("DrawWireFrame"))
//		{
//			std::string attribute(pNode->Attribute("DrawWireFrame"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_DrawWireframe;
//		}
//
//		if (pNode->Attribute("DrawAabb"))
//		{
//			std::string attribute(pNode->Attribute("DrawAabb"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_DrawAabb;
//		}
//
//		if (pNode->Attribute("DrawFeaturesText"))
//		{
//			std::string attribute(pNode->Attribute("DrawFeaturesText"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_DrawFeaturesText;
//		}
//
//		if (pNode->Attribute("DrawContactPoints"))
//		{
//			std::string attribute(pNode->Attribute("DrawContactPoints"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_DrawContactPoints;
//		}
//
//		if (pNode->Attribute("NoDeactivation"))
//		{
//			std::string attribute(pNode->Attribute("NoDeactivation"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_NoDeactivation;
//		}
//
//		if (pNode->Attribute("NoHelpText"))
//		{
//			std::string attribute(pNode->Attribute("NoHelpText"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_NoHelpText;
//		}
//
//		if (pNode->Attribute("DrawText"))
//		{
//			std::string attribute(pNode->Attribute("DrawText"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_DrawText;
//		}
//
//		if (pNode->Attribute("ProfileTimings"))
//		{
//			std::string attribute(pNode->Attribute("ProfileTimings"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_ProfileTimings;
//		}
//
//		if (pNode->Attribute("EnableSatComparison"))
//		{
//			std::string attribute(pNode->Attribute("EnableSatComparison"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_EnableSatComparison;
//		}
//
//		if (pNode->Attribute("DisableBulletLCP"))
//		{
//			std::string attribute(pNode->Attribute("DisableBulletLCP"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_DisableBulletLCP;
//		}
//
//		if (pNode->Attribute("EnableCCD"))
//		{
//			std::string attribute(pNode->Attribute("EnableCCD"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_EnableCCD;
//		}
//
//		if (pNode->Attribute("DrawConstraints"))
//		{
//			std::string attribute(pNode->Attribute("DrawConstraints"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_DrawConstraints;
//		}
//
//		if (pNode->Attribute("DrawConstraintLimits"))
//		{
//			std::string attribute(pNode->Attribute("DrawConstraintLimits"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_DrawConstraintLimits;
//		}
//
//		if (pNode->Attribute("FastWireframe"))
//		{
//			std::string attribute(pNode->Attribute("FastWireframe"));
//			if (attribute == "yes") debugModes |= btIDebugDraw::DBG_FastWireframe;
//		}
//
//		if (debugModes != btIDebugDraw::DBG_NoDebug)
//		{
//			setDebugMode(debugModes);
//		}
//	}
//
//}



void BulletPhysics::drawLine(const btVector3& from, const btVector3& to, const btVector3& lineColor)
{

	vec3 vec3From, vec3To;
	vec3From.x = from.x();
	vec3From.y = from.y();
	vec3From.z = from.z();

	vec3To.x = to.x();
	vec3To.y = to.y();
	vec3To.z = to.z();
	vec3 color(lineColor.x(), lineColor.y(), lineColor.z());


	m_pDebuger->DrawLine(vec3From, vec3To, color);
}

}
}