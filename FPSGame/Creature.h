#pragma once


class Creature : public Actor
{
public:
	Creature(ActorId id) :Actor(id) {};
	~Creature()=default;
	virtual bool	Init(const tinyxml2::XMLElement* pData);

public:

	bool			ChangeHP(int newHP);
	bool			ChangeAC(int newAC);
	bool			CanTakeDamage();
	const int		GetHP()const;
	int				GetAC();
	virtual void	TakeDamage(int damage);
	virtual void	Death() = 0;
protected:
	// heath and armor value
	int	m_HP;
	int m_AC;
	bool m_bCanTakeDamage;
};