#pragma once


class ZombieController : public ActorComponent
{
public:
	ZombieController();
	~ZombieController() = default;
	static const char*	Name;

	virtual bool VInit(const tinyxml2::XMLElement* pData);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p) { return nullptr; };
	virtual void VPostInit(void) {};
	virtual void VUpdate(float dt);
	virtual void VPostUpdate() {};
	virtual void VOnChanged(void) {};

	virtual const char *VGetName() const;


	// AI Function
	bool CanMove();
protected:
	void FindTarget(float dt);
private:
	Creature * m_pTarget; // Zombie target to attack
	Game* m_pGame; // Our game
};