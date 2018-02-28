#pragma once
#include <string>
#include <list>

#include "IGamePlugin.h"

class GamePluginManager
{
public:
	GamePluginManager(Context* pContext);
	~GamePluginManager() = default;

	bool LoadPlugin(const std::string& filename);

	void UpdateGame(float dt);
	void RenderGame();
private:
	std::list<IGamePlugin*> m_GamePlugins;
	Context* m_Context;
};
