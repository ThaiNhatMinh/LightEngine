#include <pch.h>
#include "GamePluginManager.h"

#include <Windows.h>


GamePluginManager::GamePluginManager()
{
}

GamePluginManager::~GamePluginManager()
{
}

IGamePlugin* GamePluginManager::LoadPlugin()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(PLUGIN_CONFIG_FILE);
	tinyxml2::XMLElement* pXMLNode = doc.FirstChildElement("Plugin");
	
	E_DEBUG(std::string(pXMLNode->GetText()));
	HMODULE hModule = LoadLibrary(pXMLNode->GetText());
	if (!hModule)
	{
		E_ERROR("Can't load dll: " + std::string(pXMLNode->Value()));
		return 0;
	}
	CreateInterfaceFn fnCreateInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)hModule, CREATEINTERFACE_PROCNAME);

	if (fnCreateInterface == nullptr)
	{
		E_ERROR("Can't load CreateInterface() ");
		return 0;
	}
	IGamePlugin* pGamePlugin = fnCreateInterface();

	if (pGamePlugin)
	{

		return pGamePlugin;
	}

	return nullptr;

}