#include "pch.h"
#include "GamePluginManager.h"
#include "DLLInterface.h"
#include <Windows.h>


GamePluginManager::GamePluginManager(Context * pContext):m_ContextDLL(pContext),m_GamePlugin(nullptr)
{
}

GamePluginManager::~GamePluginManager()
{
	if (m_GamePlugin) m_GamePlugin->ShutDown();
}

bool GamePluginManager::LoadPlugin(const std::string & filename)
{
	HMODULE hModule = LoadLibrary(filename.c_str());
	if (!hModule)
	{
		E_ERROR("Can't load dll: " + filename);
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
		pGamePlugin->Init(m_ContextDLL);

		m_GamePlugin = pGamePlugin;
	}

	return 1;

}

void GamePluginManager::UpdateGame(float dt)
{
	if (!m_GamePlugin) return;
	m_GamePlugin->Update(dt);
}

void GamePluginManager::RenderGame()
{
	if (!m_GamePlugin) return;
	m_GamePlugin->Render();
}

Scene * GamePluginManager::GetScene()
{
	return m_GamePlugin->GetScene();
}
