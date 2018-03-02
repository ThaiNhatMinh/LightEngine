#pragma once
#include <string>
#include <list>

#include "IGamePlugin.h"

class GamePluginManager
{
public:
	GamePluginManager(Context* pContext);
	~GamePluginManager();

	bool LoadPlugin(const std::string& filename);

	void UpdateGame(float dt);
	void RenderGame();

	Scene* GetScene();
private:
	IGamePlugin* m_GamePlugin;
	Context* m_ContextDLL;
};
