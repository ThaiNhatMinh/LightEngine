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
	virtual bool	VAddChild(std::unique_ptr<Actor> kid);
	virtual HRESULT VRenderChildren(Scene * pScene);
public:
	
	vector<LTBSocket>& GetSockets();
	void AddWeapon(Weapon* wp);
protected:
	void SetPVModel();
private:

	TeamClass	m_Team;
	string		m_Character;
	int			m_iCurrentWP;
	int			m_WPList[5];	// store index of weapon in children list
	int			Mode;
	
	Actor* PlayerView;
};