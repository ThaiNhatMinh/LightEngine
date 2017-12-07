#pragma once
#include <pch.h>

class Game : public IGame
{
public:
	virtual void Init(Context* c) ;
	virtual void Update(float dt);
	virtual void Render();
protected:

	void LoadWeapon();
	void LoadCharacter();
private:
	std::unique_ptr<Scene> m_Scene;
	std::vector<WeaponResource> m_WeaponResources;
	std::vector<CharacterResource> m_CharacterResources;
	int test[5];
};

class MyGame : public Application
{
public:
	virtual void Start()
	{
		m_Game = std::unique_ptr<Game>(new Game);
		m_Game->Init(m_Context.get());
	}

};