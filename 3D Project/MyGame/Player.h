#pragma once

#include <BulletCollision\CollisionDispatch\btGhostObject.h>
// player character

class Player : public Creature
{
public:
	Player(ActorId id);
	~Player();
	virtual bool	Init(const tinyxml2::XMLElement* pData);
	virtual void	PostInit(void);
	virtual bool	VIsVisible(Scene * pScene) const;
	virtual HRESULT VRender(Scene* pScene) override;

public:
	
	vector<LTBSocket>& GetSockets();
	void AddWeapon(Weapon* wp);
private:

	TeamClass	m_Team;
	string		m_Character;
	int			m_iCurrentWP;
	int			m_WPList[5];	// store index of weapon in children list
	
	

};