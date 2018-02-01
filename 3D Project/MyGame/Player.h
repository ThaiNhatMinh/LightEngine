#pragma once

#include <BulletCollision\CollisionDispatch\btGhostObject.h>
// player character

class Player : public Creature
{
public:
	enum PlayerState
	{
		PS_TAKEDAMAGE,
		PS_DIEING
	};
	enum charanim
	{
		jump=18
	};
	enum weaponanim
	{
		idle,
		shoot,
		reload,
		c_walk,
		c_walkBside,
		c_walkRside,
		c_walkLside,
		walk,
		walkBside,
		walkRside,
		walkLside,
		run,
		runBside,
		runRside,
		runLside,
		count

	};

	Player(ActorId id);
	~Player();
	virtual bool				Init(const tinyxml2::XMLElement* pData);
	virtual void				PostInit(void);
	virtual bool				VIsVisible(Scene * pScene) const;
	virtual HRESULT				VRender(Scene* pScene) override;
	virtual bool				VAddChild(std::unique_ptr<Actor> kid);
	virtual HRESULT				VRenderChildren(Scene * pScene);
	virtual HRESULT				VOnUpdate(Scene *, float elapsedMs);
public:
	
	const WeaponResource&		GetCurrentWeaponInfo() const;
	vector<LTBSocket>&			GetSockets();
	virtual void				Death();
	
protected:
	void SetPVModel();
private:

	
	TeamClass					m_Team;
	string						m_Character;
	int							m_iCurrentWP;
	int							m_WPList[5];	// store index of weapon in children list
	int							Mode;
	PlayerState					m_State;
	Actor						*PlayerView;
	
};