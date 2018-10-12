#pragma once
#include <string>
#include <list>

#include "..\Interface\IGamePlugin.h"

const static char* PLUGIN_CONFIG_FILE = "Configs\\Plugin.xml";

class GamePluginManager
{
private:
	class NullGamePlugin : public IGamePlugin
	{
	public:
		
		virtual void	Init(Light::IContext*) {};
		virtual void	Update(float dt) {};
		virtual void	ShutDown() {};
	};
public:
	GamePluginManager();
	~GamePluginManager();

	IGamePlugin* LoadPlugin();

private:
	NullGamePlugin m_Null;
};
