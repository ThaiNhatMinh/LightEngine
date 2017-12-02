#pragma once
#include <pch.h>

class Game : public IGame
{

public:
	virtual void Init() {};
	virtual void Update(float dt) {};
	virtual void Render() {};
};

class MyGame : public Application
{
public:
	virtual void Start()
	{
		m_Game = std::unique_ptr<Game>(new Game);
	}

};