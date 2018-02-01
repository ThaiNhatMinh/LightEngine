#pragma once

struct PhysicsRaycastResult;
class RigidBodyComponent;
class IPhysic
{
public:
	virtual ~IPhysic() {};
	virtual void RemoveActor(ActorId id) = 0;
	virtual void RayCast(PhysicsRaycastResult& result, const Ray& r, float maxdistance, unsigned mask = 0xffffffff) = 0;
	virtual void RayCast(std::vector<PhysicsRaycastResult>& result, const Ray& r, float maxdistance, unsigned mask = 0xffffffff) = 0;
	virtual btDiscreteDynamicsWorld* GetCollisionWorld()  = 0;
	virtual void AddRigidBody(ActorId id, RigidBodyComponent* rb) = 0;
};