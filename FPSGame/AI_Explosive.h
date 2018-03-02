#pragma once

class AIExplosive : public Zombie
{
private:
	enum animlist
	{
		M_Wait,
		M_Hit01,
		M_Hit02,
		M_BHit01,
		M_BHit02,
		M_Idle,
		M_Combo01,
		M_Combo02,
		M_Run,
		M_Walk,
		M_Skill01
	};
	enum State
	{
		IDLE,
		WALK,
		RUN,
		ATTACK,
		BEING_ATTACKED,
		DEATH
	};
	enum Info
	{
		EX_RANGE1 = 200,
		EX_RANGE2 = 400,
		EX_DAMAGE = 20
	};
public:
	
	AIExplosive(ActorId id);
	~AIExplosive();


	virtual void	TakeDamage(int damage) override;
	virtual void	Death()override;
	virtual HRESULT VOnUpdate(Scene * pScene, float elapsedMs);
private:

	void ExplosiveSkill();
private:
	State			m_State;
};