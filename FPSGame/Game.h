#pragma once
#include "stdafx.h"

class Game : public IGamePlugin
{
public:
	virtual void Init(Context* c) ;
	virtual void Update(float dt);
	virtual void Render();
	virtual void ShutDown();
	virtual Scene*	GetScene();

public:
	static CharacterResource LoadCharacter(const string& file);
	static WeaponResource LoadWeaponInfo(const string& wpName);
protected:
	void EventTakeDamage(std::shared_ptr<IEvent> pEvent);
	void EventExplosion(std::shared_ptr<IEvent> pEvent);
	void EventCreateActor(std::shared_ptr<IEvent> pEvent);
private:
	std::unique_ptr<Scene> m_Scene;
	std::vector<WeaponResource> m_WeaponResources;
	std::vector<CharacterResource> m_CharacterResources;
	std::vector<Player*> m_PlayerLists;

	Context* m_Context;
};