#pragma once

class IPhysic;
class IResources;
class IRenderer;
class ISystemUI;
class IWindows;
class IConsole;
class IDebugger;
class IEventManager;
class IInput;
class IEffectSystem;
class IFactory;

class IContextDLL
{
public:
	virtual IConsole*		GetConsole() = 0;
	virtual IPhysic*		GetPhysic() = 0;
	virtual IResources*		GetResources() = 0;
	virtual IRenderer*		GetRenderer() = 0;
	virtual ISystemUI*		GetSystemUI() = 0;
	virtual IWindows*		GetWindows() = 0;
	virtual IDebugger*		GetDebugger() = 0;
	virtual IEventManager*	GetEventManager() = 0;
	virtual IInput*			GetInput() = 0;
	virtual IEffectSystem*	GetEffectSystem() = 0;
	virtual IFactory*		GetFactory() = 0;

	virtual ~IContextDLL() {}
};
