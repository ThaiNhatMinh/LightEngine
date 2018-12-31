#pragma once


#include "GameState.h"
#include <IScene.h>
#include <IInput.h>
#include <IWindow.h>
#include <IVGUI.h>
#include <IContext.h>
class GS_Game: public GameState
{
public:
	GS_Game(StateStack* pOwner);

	virtual void OnEnter(StateStack * pOwner);
	virtual void OnExit(StateStack * pOwner);
	virtual bool Update(float dt);
	virtual bool OnInputEvent(Light::events::EvtInput* pEvent);
private:
	Light::vgui::IVGUI* m_pVGUI;
	Light::IScene* m_pScene;
	Light::IInput* m_pInput;
	Light::IWindow* m_pWindow;
	Light::IContext* m_pContext;
};