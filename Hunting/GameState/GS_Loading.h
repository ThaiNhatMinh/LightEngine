#pragma once
#include "GameState.h"
#include <thread>
#include <mutex>
#include <IResourceManager.h>
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

private:
	StateStack* pOwner;
	std::mutex m_Loading;
	bool m_bLoadFinish;
	Light::IContext* m_pContext;
	std::thread m_LoadThread;
};