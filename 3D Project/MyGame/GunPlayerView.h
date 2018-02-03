#pragma once


class GunPlayerView :public PlayerView
{
public:
	enum WeaponState
	{
		WS_IDLE,
		WS_BEGINSHOT,
		WS_SHOOTING,
		WS_ENDSHOOT,
		WS_RELOADING,
		WS_SELECT
	};
	enum GunAnimation
	{
		IDLE,
		PRE_FIRE,
		FIRE,
		POST_FIRE,
		RELOAD,
		RUN,
		SELECT,
		COUNT

	};

	GunPlayerView(ActorId id);
	~GunPlayerView();

	virtual HRESULT		VOnUpdate(Scene *, float elapsedMs);

	void Shoot();
	void Reload();
	void Select();
	void Run();

	void SetData(ModelCache* pModel);
protected:
	void ShootRayCast();

private:
	
	WeaponState				m_State;
	// map state to animation index
	map<GunAnimation, int>	m_AnimationMap;
	map<GunAnimation, int>	m_SoundMap;
};