#pragma once


// player character

class Player : public Actor
{
public:
	Player(ActorId id);
	~Player();
	virtual bool	Init(const tinyxml2::XMLElement* pData);
	virtual void	PostInit(void);
	virtual HRESULT VRender(Scene* pScene) override;

protected:
	void EventWeaponData(std::shared_ptr<const IEvent> pEvents);
	void EventCharacterData(std::shared_ptr<const IEvent> pEvents);
private:
	TeamClass	m_Team;
	GLuint		m_HP;
	GLuint		m_AC;
	string		m_Character;
	ModelCache* m_RModel;

	

};