#pragma once
#include "..\Interface\IEvent.h"
#include "..\Interface\IActor.h"

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
		};


		//---------------------------------------------------------------------------------------------------------------------
		// EvtData_Destroy_Actor - sent when actors are destroyed	
		//---------------------------------------------------------------------------------------------------------------------
		class EvtDestroyActor : public Event<EvtDestroyActor>
		{
			IActor* m_pActor;
		public:
			explicit EvtDestroyActor(IActor*pActor):m_pActor(pActor){}
			IActor* GetActor(void) const { return m_pActor; }
		};

		//---------------------------------------------------------------------------------------------------------------------
		// EvtData_Move_Actor - sent when actors are moved
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
	}
}