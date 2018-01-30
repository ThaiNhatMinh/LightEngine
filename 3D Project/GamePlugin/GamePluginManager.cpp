#include "pch.h"
#include "GamePluginManager.h"
#include "Interface.h"
#include <Windows.h>


GamePluginManager::GamePluginManager(Context * pContext):m_Context(pContext)
{
}

bool GamePluginManager::LoadPlugin(const std::string & filename)
{
	HMODULE hModule = LoadLibrary(filename.c_str());
	if (!hModule) return 0;

	CreateInterfaceFn fnCreateInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)hModule, CREATEINTERFACE_PROCNAME);

	IGamePlugin* pGamePlugin = fnCreateInterface();

	if (pGamePlugin)
	{
		pGamePlugin->Init(nullptr);

		m_GamePlugins.push_back(pGamePlugin);
	}

}
