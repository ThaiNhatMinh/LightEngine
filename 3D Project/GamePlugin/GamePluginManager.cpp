#include <pch.h>
#include <tinyxml2/tinyxml2.h>
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
	
	E_DEBUG(pXMLNode->GetText());
	HMODULE hModule = LoadLibrary(pXMLNode->GetText());
	if (!hModule)
	{
		E_ERROR("Can't load dll: %s" ,pXMLNode->Value());
		return &m_Null;
	}
	CreateInterfaceFn fnCreateInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)hModule, CREATEINTERFACE_PROCNAME);

	if (fnCreateInterface == nullptr)
	{
		E_ERROR("Can't load CreateInterface() ");
		return &m_Null;
	}
	IGamePlugin* pGamePlugin = fnCreateInterface();

	

	if (pGamePlugin)
	{

		return pGamePlugin;
	}

	return &m_Null;

}