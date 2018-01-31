#pragma once

#include "IContextDLL.h"

class Context;
class ContextDLL:public IContextDLL
{
public:
	ContextDLL(Context* context);
	virtual IConsole*		GetConsole();
	virtual IPhysic*		GetPhysic();
	virtual IResources*		GetResources();
	virtual IRenderer*		GetRenderer();
	virtual ISystemUI*		GetSystemUI();
	virtual IWindows*		GetWindows();
	virtual IDebugger*		GetDebugger();
	virtual IEventManager*	GetEventManager();
	virtual IInput*			GetInput();
	virtual IEffectSystem*	GetEffectSystem();
	virtual IFactory*		GetFactory();
private:
	Context* m_Context;
};