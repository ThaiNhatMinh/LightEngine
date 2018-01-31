#pragma once
#include <string>
#include <list>

#include "IGamePlugin.h"
#include "ContextDLL.h"

class GamePluginManager
{
public:
	GamePluginManager()=default;
	GamePluginManager(Context* pContext);
	~GamePluginManager() = default;

	bool LoadPlugin(const std::string& filename);

	void UpdateGame(float dt);
	void RenderGame();
private:
	std::list<IGamePlugin*> m_GamePlugins;
	ContextDLL m_ContextDLL;
};
