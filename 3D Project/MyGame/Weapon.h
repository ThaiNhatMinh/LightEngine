#pragma once

class Weapon :public Actor
{
	friend class Player;
public:
	Weapon(ActorId id);
	virtual bool	Init(const tinyxml2::XMLElement* pData);
	virtual void	PostInit(void);
	virtual HRESULT VRender(Scene* pScene) override;
	
	int GetWeaponIndex();
	
private:
	void LoadData(const WeaponResource & wr);
	int m_Index;
};