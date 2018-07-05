#pragma once

class AIExplosive;

class ExplosiveIdle :public State<AIExplosive>
{
public:
	ExplosiveIdle() = default;

	virtual void Enter(AIExplosive*) override;
	virtual void Execute(AIExplosive *pZombie) override;
	virtual void Exit(AIExplosive *pZombie) override {};

private:
	ZombieController * m_pController;
};

class ExplosiveDeath : public State<AIExplosive>
{
public: 
	ExplosiveDeath() = default;
	virtual void Enter(AIExplosive*) override;
	virtual void Execute(AIExplosive *pZombie) override;
	virtual void Exit(AIExplosive *pZombie) override {};

private:
	AnimationComponent * m_pAnimComponent;
	SoundSource3D* m_pSoundComponent;
};


class ExplosiveRunning : public State<AIExplosive>
{
public:
	ExplosiveRunning() = default; 
	virtual void Enter(AIExplosive*) override ;
	virtual void Execute(AIExplosive *pZombie) override;
	virtual void Exit(AIExplosive *pZombie) override {};

private:
	ZombieController * m_pController;

};

class AIExplosive : public Zombie,public StateMachine<AIExplosive>
{
private:
	friend class ExplosiveIdle;
	friend class ExplosiveDeath;
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
	/*enum Statea
	{
		IDLE,
		WALK,
		RUN,
		ATTACK,
		BEING_ATTACKED,
		DEATH
	};*/
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
	
	
};