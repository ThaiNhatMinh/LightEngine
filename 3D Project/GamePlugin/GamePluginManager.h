#pragma once
#include <string>
#include <list>

#include "..\Interface\IGamePlugin.h"

class GamePluginManager
{
	const char* PLUGIN_CONFIG_FILE = "Configs\\Plugin.xml";
public:
	GamePluginManager();
	~GamePluginManager();

	IGamePlugin* LoadPlugin();

private:
	
};
