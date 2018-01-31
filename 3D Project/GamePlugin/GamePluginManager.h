#pragma once
#include <string>
#include <list>

#include "IGamePlugin.h"

class GamePluginManager
{
public:
	GamePluginManager()=default;
	GamePluginManager(Context* pContext);
	~GamePluginManager() = default;

	bool LoadPlugin(const std::string& filename);

private:
	std::list<IGamePlugin*> m_GamePlugins;
	Context* m_Context;
};
