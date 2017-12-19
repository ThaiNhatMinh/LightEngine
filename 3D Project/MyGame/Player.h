#pragma once


// player character

class Player : public Actor
{
public:
	Player(ActorId id);
	~Player();
	virtual bool	Init(const tinyxml2::XMLElement* pData);
	virtual void	PostInit(void);
	virtual bool	VIsVisible(Scene * pScene) const;
	virtual HRESULT VRender(Scene* pScene) override;

protected:
	void EventWeaponData(std::shared_ptr<const IEvent> pEvents);
	void EventCharacterData(std::shared_ptr<const IEvent> pEvents);
private:

	BaseAnimComponent* m_AnimC;
	std::unique_ptr<MeshRenderComponent> m_MeshRender;

	TeamClass	m_Team;
	GLuint		m_HP;
	GLuint		m_AC;
	string		m_Character;
	ModelCache* m_RModel;
	int			m_iCurrentWP;
	int			m_WPList[5];	// store index of weapon in children list
	
	

};