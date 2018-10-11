#pragma once
#include "..\Interface\IEvent.h"
#include "..\Interface\IActor.h"
#include "..\Graphics3D\ICamera.h"
#include "..\Interface\IScene.h"
#include "..\Interface\IInput.h"
#include "..\Interface\IGamePhysic.h"


namespace Light
{
	namespace events
	{
		class EvtNewActor : public Event<EvtNewActor>
		{
			IActor* m_pActor;
		public:
			explicit EvtNewActor(IActor* pActor) :m_pActor(pActor){	}
			IActor* GetActor(void) const{return m_pActor;}
			ActorId GetID() {return m_pActor->VGetId();	};
		};


		//---------------------------------------------------------------------------------------------------------------------
		// EvtDestroyActor - sent when actors are destroyed	
		//---------------------------------------------------------------------------------------------------------------------
		class EvtDestroyActor : public Event<EvtDestroyActor>
		{
			ActorId m_Actor;
		public:
			explicit EvtDestroyActor(ActorId pActor):m_Actor(pActor){}
			ActorId GetActor(void) const { return m_Actor; }
		};

		//---------------------------------------------------------------------------------------------------------------------
		// EvtMoveActor - sent when actors are moved
		//---------------------------------------------------------------------------------------------------------------------
		class EvtMoveActor : public Event<EvtMoveActor>
		{
			IActor* m_actor;
			glm::mat4 m_matrix;

		public:
			EvtMoveActor(IActor* id, const glm::mat4& matrix): m_actor(id), m_matrix(matrix){}
			IActor* GetId(void) const{	return m_actor;}
			const glm::mat4& GetMatrix(void) const{	return m_matrix;}
		};

		//---------------------------------------------------------------------------------------------------------------------
		// EvtCameraCreate - sent when Camera is create
		//---------------------------------------------------------------------------------------------------------------------
		class EvtCameraCreate : public Event<EvtCameraCreate>
		{
			render::ICamera* m_pCamera;

		public:
			EvtCameraCreate(render::ICamera* camera) : m_pCamera(camera){}
			render::ICamera* GetCamera() { return m_pCamera; }
		};

		class EvtSceneCreate : public Event<EvtSceneCreate>
		{
		public:
			IScene* m_pScene;
			EvtSceneCreate(IScene* pScene) :m_pScene(pScene) {}

		};

		class EvtKeyEvent : public Event<EvtKeyEvent>
		{
		public:
			KeyAction action;
			Key key;
			float dt;

		};
		class EvtMouseMove : public Event<EvtMouseMove>
		{
		public:
			float dx, dy;
			float x, y;

		};

		class EvtPhysPostStep : public Event<EvtPhysPostStep>
		{
		public:
			float m_fTimeStep;
			EvtPhysPostStep(float t) :m_fTimeStep(t) {};
		};

		class EvtPhysPreStep : public Event<EvtPhysPreStep>
		{
		public:
			float m_fTimeStep;
			EvtPhysPreStep(float t) :m_fTimeStep(t) {};
		};

		class EvtPhysCollisionEnd : public Event<EvtPhysCollisionEnd>
		{
		public:
			ActorId m_ActorA;
			ActorId m_ActorB;
			EvtPhysCollisionEnd(ActorId actorA, ActorId actorB)
				: m_ActorA(actorA)
				, m_ActorB(actorB) {};
		};


		class EvtPhysOnCollision : public Event<EvtPhysOnCollision>
		{
		public:
			IActor* m_ActorA;
			IActor* m_ActorB;

			vec3 m_SumNormalForce;
			vec3 m_SumFrictionForce;
			vector<physics::PhysicCollisionData> m_CollisionData;
			EvtPhysOnCollision(IActor* actorA,
				IActor* actorB,
				vec3 sumNormalForce,
				vec3 sumFrictionForce,
				vector<physics::PhysicCollisionData> collisionPoints)
				: m_ActorA(actorA),
				m_ActorB(actorB),
				m_SumNormalForce(sumNormalForce),
				m_SumFrictionForce(sumFrictionForce),
				m_CollisionData(collisionPoints) {};
		};
		class EvtPhysCollisionStart : public Event<EvtPhysCollisionStart>
		{
		public:
			IActor* m_ActorA;
			IActor* m_ActorB;
			vec3 m_SumNormalForce;
			vec3 m_SumFrictionForce;
			vector<physics::PhysicCollisionData> m_CollisionPoints;
			EvtPhysCollisionStart(IActor* actorA,
				IActor* actorB,
				vec3 sumNormalForce,
				vec3 sumFrictionForce,
				vector<physics::PhysicCollisionData> collisionPoints)
				: m_ActorA(actorA),
				m_ActorB(actorB),
				m_SumNormalForce(sumNormalForce),
				m_SumFrictionForce(sumFrictionForce),
				m_CollisionPoints(collisionPoints)
			{};
		};
		class EvtPhysTriggerLeave : public Event<EvtPhysTriggerLeave>
		{
		public:
			int m_triggerID;
			ActorId m_other;
			EvtPhysTriggerLeave(int triggerID, ActorId other)
				: m_triggerID(triggerID),
				m_other(other) {};
		};

		class EvtPhysTriggerEnter : public Event<EvtPhysTriggerEnter>
		{
		public:
			int m_triggerID;
			ActorId m_other;
			EvtPhysTriggerEnter(int triggerID, ActorId other)
				: m_triggerID(triggerID),
				m_other(other) {};
		};


		class EvtRequestCreateSprite :public Event<EvtRequestCreateSprite>
		{

		public:
			bool Loop;
			string File;
			IActor* Parent;
			glm::vec3 Pos;
		};
	}
}