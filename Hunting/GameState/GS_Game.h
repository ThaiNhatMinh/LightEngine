#pragma once


#include "GameState.h"
#include <IScene.h>
#include <IVGUI.h>
class GS_Game: public GameState
{
public:
	GS_Game(StateStack* pOwner);

	virtual void OnEnter(StateStack * pOwner);
	virtual void OnExit(StateStack * pOwner);
	virtual bool Update(float dt);

private:
	Light::vgui::IVGUI* m_pVGUI;
	Light::IScene* m_pScene;
};