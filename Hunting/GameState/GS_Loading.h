#pragma once
#include "GameState.h"
#include <thread>
#include <mutex>
#include <IResourceManager.h>
#include <IVGUI.h>
#include <IContext.h>
class GS_Loading: public GameState
{
public:
	GS_Loading(StateStack* pOwner);
	~GS_Loading();
	


	virtual void OnEnter(StateStack * pOwner);
	virtual void OnExit(StateStack * pOwner);
	virtual bool Update(float dt);
private:

	void LoadingFunc(Light::resources::IResourceManager* resources, StateStack * pOwner);
	void SetStatus(int s);
private:
	Light::vgui::IVGUI* m_pVGUI;
	StateStack* pOwner;
	std::mutex m_Loading,m_Status;
	
	bool m_bLoadFinish;
	int status;
	Light::IContext* m_pContext;
	std::thread m_LoadThread;
};