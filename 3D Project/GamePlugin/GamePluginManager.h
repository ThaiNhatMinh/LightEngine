#pragma once
#include <string>
#include <list>

#include "..\Interface\IGamePlugin.h"

const static char* PLUGIN_CONFIG_FILE = "Configs\\Plugin.xml";

class GamePluginManager
{
public:
	
public:
	GamePluginManager();
	~GamePluginManager();

	IGamePlugin* LoadPlugin();

private:
	
};
